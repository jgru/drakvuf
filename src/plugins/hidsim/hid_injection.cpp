
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // usleep
#include <sys/time.h>
#include <linux/input.h>

/* Hidsim specific includes */
#include "qmp_connection.h"
#include "hid_injection.h"

#define CMD_BUF_LEN 0x400

struct dimensions
{
    int width;      // display width
    int height;     // display height
    float dx;       // one pixel equivalent on x-axis
    float dy;       // one pixel equivalent on y-axis
};

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

/* Constructs JSON string */
void construct_button_command(char* buf, int len, const char* btn, int is_down)
{
    const char* type = is_down ? "true" : "false";
    snprintf(buf, len, 
    "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [ \
    { \"type\": \"btn\" , \"data\" : { \"down\": %s, \"button\": \"%s\" } }\
     ] } }", type, btn);
}

/* Constructs JSON string */
void construct_move_mouse_command(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf, len, "{\"execute\": \"input-send-event\" , \"arguments\": \
    {\"events\": [{\"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": \
    %d }}, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d }}]}}",
        type, x, type, y);
}

/* Helper to center cursor */
void center_cursor(qmp_connection* qc)
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];

    /* 2^15 == max, 2^14 == max/2 */
    construct_move_mouse_command(buf, CMD_BUF_LEN, 1<<14, 1<<14, false);
    qmp_communicate(qc, buf, NULL);
}

int read_header(FILE* f)
{
    uint32_t magic, drak, version;
    int nr = 0;

    nr += fread(&magic, sizeof(magic), 1, f);
    nr += fread(&drak, sizeof(drak), 1, f);
    nr += fread(&version, sizeof(version), 1, f);

    if (nr < 3)
    {
        fprintf(stderr, "[HIDSIM] Error reading file header of HID-template");
        return 1;
    }
    return 0;
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

int run_template_injection(qmp_connection* qc, FILE* f, bool* has_to_stop)
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];
    struct input_event ie;

    size_t bytes_read = 0;
    int new_x, new_y = 0;

    struct timeval tv_old;
    struct timeval tv_diff;
    struct timeval tv_curr;
    struct timeval tv_last_qmp;
    struct timeval tv_last_qmp_diff;
    bool is_wait = false;

    while (!*has_to_stop)
    {
        if (bytes_read == 0)
        {
            /* Prepares injection by initializing all variables */
            if (reset_hid_injection(qc, f, &tv_old, &new_x, &new_y) != 0)
                fprintf(stderr, "[HIDSIM] Error resetting HID injection");
        }

        bytes_read = fread(&ie, sizeof(ie), 1, f);

        /* Handles move events */
        if (ie.type == EV_REL)
        {
            if (ie.code == REL_X)
            {
                new_x += ie.value;
            }
            if (ie.code == REL_Y)
            {
                new_y += ie.value;
            }
        }

        /* Handles click events */
        if (ie.type == EV_KEY)
        {
            if (ie.code == BTN_LEFT)
            {
                construct_button_command(buf, CMD_BUF_LEN, "left", ie.value);
            }
            if (ie.code == BTN_MIDDLE)
            {
                construct_button_command(buf, CMD_BUF_LEN, "middle", ie.value);
            }
            if (ie.code == BTN_RIGHT)
            {
                construct_button_command(buf, CMD_BUF_LEN, "right", ie.value);

            }
            qmp_communicate(qc, buf, NULL);
            gettimeofday(&tv_last_qmp, NULL);
        }

        timersub(&ie.time, &tv_old, &tv_diff);

        if (tv_diff.tv_sec > 0)
        {
            sleep(tv_diff.tv_sec);
            is_wait = true;
        }

        if (tv_diff.tv_usec > 0)
        {
            usleep(tv_diff.tv_usec);
            is_wait = true;
        }

        gettimeofday(&tv_curr, NULL);
        timersub(&tv_curr, &tv_last_qmp, &tv_last_qmp_diff);

        /* Throttles the sending of QMP commands */
        if (is_wait && tv_last_qmp_diff.tv_usec > 5000)
        {
            construct_move_mouse_command(buf, CMD_BUF_LEN, new_x, new_y, false);
            qmp_communicate(qc, buf, NULL);
            gettimeofday(&tv_last_qmp, NULL);
            is_wait = false;
        }

        tv_old.tv_sec = ie.time.tv_sec;
        tv_old.tv_usec = ie.time.tv_usec;
    }

    return 0;
}

void* run_random_injection(qmp_connection* qc, bool* has_to_stop)
{
    struct dimensions dim;
    get_display_dimensions(qc, &dim);

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
            construct_move_mouse_command(buf, CMD_BUF_LEN, (int)((1<<14) + (128 * dim.dx * sign)),
                (int)((1<<14) + (128 *  dim.dy * sign)), false);
            qmp_communicate(qc, buf, NULL);
            sign = ~sign + 1;
            gettimeofday(&t1, NULL);
        }
        else
        {
            /* Sleeps until waiting interval is over */
            usleep(uinterval - elapsed);
        }
    }
    return NULL;
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

    if (qmp_init_conn(&qc, sock_path) < 0)
    {
        fprintf(stderr, "[HIDSIM] Could not connect to Unix Domain Socket %s.\n", sock_path);
        return NULL;
    }

    if (template_path){
        FILE *f = fopen(template_path, "rb");

        if (!f)
        {
            fprintf(stderr, "[HIDSIM] Error reading from %s\n", template_path);
            return NULL;
        }

        if (read_header(f) != 0)
        {
            fprintf(stderr, "[HIDSIM] Not a valid HID template file. Stopping");
            return NULL;
        }

        /* Performs actual injection */
        run_template_injection(&qc, f, has_to_stop);
        
        fclose(f);
    }
    else
    {
        run_random_injection(&qc, has_to_stop);
    }

    /* Stop and clean up */
    qmp_close_conn(&qc);

    return NULL;
}
