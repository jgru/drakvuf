
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>

#include <libdrakvuf/libdrakvuf.h> /* eprint_current_time */
#include "../private.h" /* PRINT_DEBUG */

/* Hidsim specific includes */
#include "keymap_evdev_to_qapi.h" /* Mapping evdev->qapi name */
#include "qmp_connection.h"
#include "hid_injection.h"

#define DRAK_MAGIC_NUMBER 0xc4d2c1cb
#define DRAK_MAGIC_STR 0x4b415244
#define CMD_BUF_LEN 0x400
#define TIME_BIN 50

struct dimensions
{
    int width;      // display width
    int height;     // display height
    float dx;       // one pixel equivalent on x-axis
    float dy;       // one pixel equivalent on y-axis
};

/* Keep track of the current cursor position */
int new_x = 1 << 14;
int new_y = 1 << 14;

int dump_screen(qmp_connection* qc, const char* path)
{
    char cmd[0x200];
    snprintf(cmd, 0x200,
        "{ \"execute\":\"screendump\", \"arguments\": { \"filename\": \"%s\" } }",
        path);
    char* out = NULL;

    qmp_communicate(qc, (const char*) cmd, &out);

    if (strcmp(out, QMP_SUCCESS) != 0)
    {
        fprintf(stderr, "[HIDSIM]: Error dumping screen - %s\n", out);
    }
    return 0;
}

/* For absolute pointing devices 2^15 is used to specify maximum */
float calculate_pixel_unit(int u)
{
    return (float)(1<<15)/u;
}

/* Retrieves display dimensions for coordination of mouse movements */
int get_display_dimensions(qmp_connection* qc, struct dimensions* dims)
{
    const char* tmp_path = "/tmp/tmp.ppm";

    /* Dumps screen via QMP-command and stores result at tmp_path */
    if (dump_screen(qc, tmp_path) != 0)
        return -1;

    /* Extracts display size from .ppm-file created by screendump */
    FILE* fr = fopen(tmp_path, "r");
    char ppm_hdr[3];
    int matches = 0;
    int ret = 0;

    /* Checks PPM-header */
    matches = fscanf(fr, "%s", ppm_hdr);

    if (matches != 1 || strcmp(ppm_hdr, "P6") != 0)
        ret = -1;

    /* Reads actual dimensions */
    matches = fscanf(fr, "%d%d", &(dims->width), &(dims->height));

    /* Calculates pixel mapping */
    dims->dx = calculate_pixel_unit(dims->width);
    dims->dy = calculate_pixel_unit(dims->height);

    if (matches != 2 || dims->width < 0 || dims->height < 0)
        ret = -1;

    /* Clean up */
    if (remove(tmp_path) != 0)
        fprintf(stderr, "[HIDSIM]: Error deleting screen dummp %s\n", tmp_path);

    return ret;
}

/* Parses header of HID-template-file */
int read_header(FILE* f)
{
    uint32_t magic, drak, version;
    int nr = 0;

    nr += fread(&magic, sizeof(magic), 1, f);
    nr += fread(&drak, sizeof(drak), 1, f);
    nr += fread(&version, sizeof(version), 1, f);
    if (nr < 3)
    {
        fprintf(stderr, "[HIDSIM] Generoc error reading file header of HID-template\n");
        return 1;
    }

    if (magic != DRAK_MAGIC_NUMBER || drak != DRAK_MAGIC_STR)
    {
        fprintf(stderr, "[HIDSIM] Error parsing header of HID-template\n");
        return 1;
    }

    return 0;
}

/* Constructs event-string to intruct a mouse movement */
void construct_move_mouse_event(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf + strlen(buf), len - strlen(buf), " { \"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": \
    %d } }, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d } }",
        type, x, type, y);
}

/* Constructs event-string for sending a button press or release */
void construct_button_event(char* buf, int len, const char* btn, int is_down, bool is_append)
{
    const char* type = is_down ? "true" : "false";
    snprintf(buf + strlen(buf), len + strlen(buf),
        " { \"type\": \"btn\" , \"data\" : { \"down\": %s, \"button\": \"%s\" } } %c",
        type, btn, is_append ? ',' : ' ');
}

/* Constructs event-string to send key down or up-command */
void construct_key_event(char* buf, int len, const unsigned int key, bool is_down, bool is_append)
{
    /*
     * For some reason sending QKeyCodes as their numbers does not work reliably.
     * Therefore it is required to map EvDev codes to the QAPI-names of QKeyCodes,
     * which QMP understands
     *
     */
    const char* state = is_down ? "true" : "false";
    const char* qapi_name = NULL;


    if (key < name_map_linux_to_qcode_len)
    {
        /*
         * Since raw values do not reliably work in (at least) Windows guests,
         *convert to qapi-names of QKeyCodes!
         */
        qapi_name = name_map_linux_to_qcode[key];
        snprintf(buf + strlen(buf), len - strlen(buf),
            " { \"type\": \"key\", \"data\" : { \"down\": %s, \"key\": {\"type\": \"qcode\", \"data\": \"%s\" } } } %c ",
            state, qapi_name, is_append ? ',' : ' ');
    }
    else
        /* Send raw evdev representation as fallback */
        snprintf(buf + strlen(buf), len - strlen(buf),
            " { \"type\": \"key\", \"data\" : { \"down\": %s, \"key\": {\"type\": \"number\", \"data\": %d } } } %c ",
            state, key, is_append ? ',' : ' ');
}

/* Helper to center cursor */
void center_cursor(qmp_connection* qc)
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];

    /* 2^15 == max, 2^14 == max/2 */
    construct_move_mouse_event(buf, CMD_BUF_LEN, 1<<14, 1<<14, false);
    qmp_communicate(qc, buf, NULL);
}

int reset_hid_injection(qmp_connection* qc, FILE* f, struct timeval* tv, int* nx, int* ny)
{
    /* Jumps to the beginning of the HID data */
    int ret = fseek(f, HEADER_LEN, SEEK_SET);

    /* Center coords */
    *nx = 1 << 14;
    *ny = 1 << 14;
    center_cursor(qc);

    timerclear(tv);

    return ret;
}
/* Processes evdev-events, which encode keypresses/-releases */
void handle_key_event(input_event* ie, char* buf, size_t n, bool is_append)
{
    /* Ignores value 2 -> key still pressed */
    if (ie->value == 0  || ie->value == 1)
        construct_key_event(buf, n, ie->code, (const unsigned int) ie->value, is_append);
}

/* Processes evdev-events, which encode mouse button presses/releases */
void handle_btn_event(input_event* ie, char* buf, size_t n, bool is_append)
{
    if (ie->code == BTN_LEFT)
    {
        construct_button_event(buf, n, "left", ie->value, is_append);
    }
    if (ie->code == BTN_MIDDLE)
    {
        construct_button_event(buf, n, "middle", ie->value, is_append);
    }
    if (ie->code == BTN_RIGHT)
    {
        construct_button_event(buf, n, "right", ie->value, is_append);
    }
}

/* Processes evdev-events, which encode mouse movements */
void handle_move_event(input_event* ie, char* buf, size_t n, int* nx, int* ny, bool is_append)
{
    if (ie->code == REL_X)
    {
        *nx += ie->value;
    }
    if (ie->code == REL_Y)
    {
        *ny += ie->value;
    }
    /*
     * For mouse movements only coords have to be updated,
     * when appending is requested
     */
    if (!is_append)
        construct_move_mouse_event(buf, n, *nx, *ny, false);
}

void handle_event(input_event* ie, char* buf, size_t n, bool is_append)
{
    /* Handles mouse move events */
    if (ie->type == EV_REL)
    {
        /* Converts to absolute coordinates */
        handle_move_event(ie, buf, n, &new_x, &new_y, is_append);

    }
    if (ie->type == EV_KEY)
    {
        if (ie->code < 0x100)
            handle_key_event(ie, buf, n, is_append);

        if (ie->code > 255 && ie->code < 0x120)
            handle_btn_event(ie, buf, n, is_append);
    }
}

int run_template_injection(qmp_connection* qc, FILE* f, bool* has_to_stop)
{
    fprintf(stderr, "[HIDSIM] running template injection\n");
    /* Command buffer */
    char cmd[CMD_BUF_LEN];
    char buf[CMD_BUF_LEN];
    struct input_event ie_next, ie_cur;

    size_t nr = 0;

    struct timeval tv_old;
    struct timeval tv_diff;

    long int sleep = 0;
    bool was_last = true;

    /* Reads first event */
    nr = fread(&ie_cur, sizeof(ie_cur), 1, f);

    /* Waits until first event should be injected */
    sleep = ie_cur.time.tv_sec * 1000000 * ie_cur.time.tv_usec;
    usleep(sleep);

    while (!*has_to_stop)
    {
        /* Handles the special case of the first event */
        if (was_last)
        {
            /* Prepares injection by initializing all variables */
            if (reset_hid_injection(qc, f, &tv_old, &new_x, &new_y) != 0)
            {
                fprintf(stderr, "[HIDSIM] Error resetting HID injection");
                return 1;
            }
            was_last = false;
        }

        /* Reads the follow-up event */
        nr = fread(&ie_next, sizeof(ie_next), 1, f);

        if (nr == 0) /* EOF was reached */
        {
            /* Iteration deals with last event in template */
            was_last = true;
            /* Resets time for the repeat */
            timerclear(&ie_cur.time);
            /* Ensures injection of pending events */
            sleep = TIME_BIN +1;
        }
        else
        {
            timersub(&ie_next.time, &ie_cur.time, &tv_diff);
            sleep = tv_diff.tv_sec * 1000000 + tv_diff.tv_usec;
        }
        /*
         * Throttles event injection, by combing all events within a timeframe of 50 usecs
         * This performs actually a binning with bin size of 50 usecs
         */
        if ( sleep > TIME_BIN)
        {
            /* Constructs event-string corresponding to evdev-event in question */
            handle_event(&ie_cur, buf, CMD_BUF_LEN, false);

            /* Constructs execute-buffer containing various events */
            snprintf(cmd, CMD_BUF_LEN, "%s", "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [");
            snprintf(cmd + strlen(cmd), CMD_BUF_LEN - strlen(cmd), "%s", buf);
            snprintf(cmd + strlen(cmd), CMD_BUF_LEN - strlen(cmd), "%s", "] } }");

            /* Sends command buffer */
            qmp_communicate(qc, cmd, NULL);
            PRINT_DEBUG("[HIDSIM] %s\n", cmd);

            /* Resets event buffer */
            buf[0] = '\0';
        }
        else
        {
            /* Converts evdev-event to qmp-string and appends it to buffer */
            handle_event(&ie_cur, buf, CMD_BUF_LEN, true);
        }
        /* Omits sleeping after injection of last event */
        if (!was_last)
        {
            usleep(sleep);
            ie_cur = ie_next;
        }
    }
    return 0;
}

int run_random_injection(qmp_connection* qc, bool* has_to_stop)
{
    struct dimensions dim;
    if (get_display_dimensions(qc, &dim) != 0)
        return -1;

    /* Converts interval to usecs */
    int uinterval = RAND_INTERVAL * 1000;

    /* Declares points in time */
    struct timeval t1, t2, tdiff;

    /* Command buffer */
    char buf[CMD_BUF_LEN];

    center_cursor(qc);
    gettimeofday(&t1, NULL);

    /* Used for inverting mouse movement */
    int sign = 0x1;

    long int elapsed = 0;

    /* Loops, until stopped */
    while (!*has_to_stop)
    {
        gettimeofday(&t2, NULL);
        timersub(&t2, &t1, &tdiff);
        elapsed = tdiff.tv_sec * 1000000 + tdiff.tv_usec;

        if (elapsed > uinterval)
        {
            snprintf(buf, CMD_BUF_LEN, "%s", "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [");

            new_x = (int)((1<<14) + (128 * dim.dx * sign));
            new_y = (int)((1<<14) + (128 *  dim.dy * sign));

            construct_move_mouse_event(buf, CMD_BUF_LEN, new_x, new_y, false);
            snprintf(buf + strlen(buf), CMD_BUF_LEN - strlen(buf), "%s", "] } }");

            qmp_communicate(qc, buf, NULL);
            fprintf(stderr, "[HIDSIM] %s\n", buf);

            sign = ~sign + 1;
            buf[0] = '\0';
            gettimeofday(&t1, NULL);
        }
        else
        {
            /* Sleeps until waiting interval is over */
            usleep(uinterval - elapsed);
        }
    }
    return 0;
}

int cleanup_template(qmp_connection* qc, int fd, FILE* f)
{
    int ret = 0;

    if (qc)
        if ((ret = qmp_close_conn(qc)) != 0)
            fprintf(stderr, "[HIDSIM] Error closing QMP socket %s\n", qc->sa.sun_path);
    if (f)
        if ((ret = fclose(f)) != 0)
            fprintf(stderr, "[HIDSIM] Error closing %p", f);
    if (fd >= 0)
        if ((ret = close(fd)) != 0)
            fprintf(stderr, "[HIDSIM] Error closing %d\n", fd);

    return ret;
}

/* Worker thread function */
void* hid_inject(void* p)
{

    /* Passed parameters */
    struct t_args* ta = (struct t_args*) p;
    const char* sock_path = ta->socket_path;
    const char* template_path = ta->template_path;
    bool* has_to_stop = ta->has_to_stop;

    /* Initializes qmp connection */
    qmp_connection qc;
    int sc = 0;
    int fd = -1;
    FILE* f = NULL;

    if (qmp_init_conn(&qc, sock_path) < 0)
    {
        fprintf(stderr, "[HIDSIM] Could not connect to Unix Domain Socket %s.\n", sock_path);
        return NULL;
    }

    if (template_path)
    {
        fd = open(template_path, O_RDONLY);

        if (fd < 0)
        {
            fprintf(stderr, "[HIDSIM] Error opening file %s\n", template_path);
            cleanup_template(&qc, fd, NULL);
            return NULL;
        }

        /* Asks for aggressive readahead */
        if (posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL)!=0)
        {
            fprintf(stderr, "[HIDSIM] Asking for aggressive readahead on FD %d failed. Continuing anyway...\n", fd);
        }

        f = fdopen(fd, "rb");

        if (!f)
        {
            fprintf(stderr, "[HIDSIM] Error reading from %s\n", template_path);
            cleanup_template(&qc, fd, NULL);
            return NULL;
        }

        if (read_header(f) != 0)
        {
            fprintf(stderr, "[HIDSIM] Not a valid HID template file. Stopping\n");
            cleanup_template(&qc, fd, f);
            return NULL;
        }

        /* Performs actual injection */
        sc = run_template_injection(&qc, f, has_to_stop);
    }
    else
    {
        sc = run_random_injection(&qc, has_to_stop);
    }
    if (sc != 0)
        fprintf(stderr, "[HIDSIM] Error performing HID injection\n");

    cleanup_template(&qc, fd, f);

    return NULL;
}
