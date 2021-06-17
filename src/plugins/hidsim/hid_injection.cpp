
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // usleep
#include <sys/time.h>
#include<fcntl.h>
#include <linux/input.h>
#include <stdbool.h>

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

/* Constructs JSON string for sending a button press or release */
void construct_button_command(char* buf, int len, const char* btn, int is_down)
{
    const char* type = is_down ? "true" : "false";
    snprintf(buf, len, 
    "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [ \
    { \"type\": \"btn\" , \"data\" : { \"down\": %s, \"button\": \"%s\" } }\
     ] } }", type, btn);
}

/* Constructs JSON string to intruct a mouse movement */
void construct_move_mouse_command(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf, len, "{\"execute\": \"input-send-event\" , \"arguments\": \
    {\"events\": [{\"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": \
    %d }}, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d }}]}}",
        type, x, type, y);
}

/* Constructs JSON string to send a key tap of 100ms */
void construct_key_tap(char* buf, int len, int* keys, size_t n)
{   
    char* cur = buf; 
    cur += snprintf(buf, len, "%s", "{\"execute\": \"send-key\" , \"arguments\": {\"keys\": [ ");

    for(size_t i = 0; i<n; i++){
        cur += snprintf(cur, len - strlen(cur), " { \"type\": \"number\", \"data\": %d }%c ", keys[i], i < n-1 ? ',' : ' ');
    }
    snprintf(cur, len - strlen(cur), "%s", "] } }");
}

/* Constructs JSON string to send key down or up-command */
void construct_key_press(char* buf, int len, int* keys, bool* states, size_t n)
{   
    char* cur = buf; 
    cur += snprintf(buf, len, "%s", "{\"execute\": \"input-send-event\" , \"arguments\": {\"events\": [ ");
    

    for(size_t i = 0; i<n; i++){
        const char* state = states[i] ? "true" : "false";
        cur += snprintf(cur, len - strlen(cur), " { \"type\": \"key\", \"data\" : { \"down\": %s, \"key\": {\"type\": \"number\", \"data\": %d } } } %c ", 
        state, keys[i], i < n-1 ? ',' : ' ');
    }
    
    snprintf(cur, len - strlen(cur), "%s", "] } }");
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
/* Constructs JSON string to intruct a mouse movement */
void construct_move_mouse_event(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf + strlen(buf), len - strlen(buf), "{\"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": \
    %d }}, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d }}",
        type, x, type, y);
}

/* Constructs JSON string for sending a button press or release */
void construct_button_event(char* buf, int len, const char* btn, int is_down, bool is_append)
{
    const char* type = is_down ? "true" : "false";
    snprintf(buf + strlen(buf), len + strlen(buf),
             " { \"type\": \"btn\" , \"data\" : { \"down\": %s, \"button\": \"%s\" } } %c",
             type, btn, is_append ? ',' : ' ');
}

ARRAY MAPPING TODO
/* Constructs JSON string to send key down or up-command */
void construct_key_event(char* buf, int len, int key, bool is_down, bool is_append)
{   
    /* 
     * For some reason sending q_key_codes as numbers does not work.
     * Therefore it is required to map evdev to the qkeycode string representation
     * which QMP understands
     * 
     * String representations are defined here: 
     * https://github.com/qemu/qemu/blob/18e53dff939898c6dd00d206a3c2f5cd3d6669db/qapi/ui.json#L799
     */
    const char *state = is_down ? "true" : "false";
    bool is_qcode = true; 
    switch (key)
    {
        case KEY_LEFTCTRL:
            /* code */
            break;
        case KEY_RIGHTCTRL:
            /* code */
            break;
        case KEY_LEFTALT:
            /* code */
            break;
        case KEY_RIGHTALT:
            /* code */
            break;
        case KEY_LEFTMETA:
            /* code */
            break;
        case KEY_RIGHTMETA:
            /* code */
            break;
        case KEY_DELETE:
            /* code */
            break;
        default:
            is_qcode = false; 
            break;
    }
    /* Raw values do not work on windows, therefore convert to QKeyCodes! */
    if(is_qcode){
        snprintf(buf + strlen(buf), len - strlen(buf), " { \"type\": \"key\", \"data\" : { \"down\": %s, \"key\": {\"type\": \"qcode\", \"data\": \"%s\" } } } %c ",
                 state, "ctrl", is_append ? ',' : ' ');
    }
    else
        snprintf(buf + strlen(buf), len - strlen(buf), " { \"type\": \"key\", \"data\" : { \"down\": %s, \"key\": {\"type\": \"number\", \"data\": %d } } } %c ",
                 state, key, is_append ? ',' : ' ');
        
}

void handle_key_event(input_event* ie, char* buf, size_t n, bool is_append){
    /* Ignores value 2 -> key still pressed */
    if(ie->value == 0  || ie->value == 1)
        construct_key_event(buf, n, ie->code, ie->value, is_append); 
}

void handle_btn_event(input_event* ie, char* buf, size_t n, bool is_append){
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

static int new_x = 1 << 14;
static int new_y = 1 << 14;

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
    //fprintf(stderr, "%d, %d\n", *new_x, *new_y);

    /* 
     * For mouse movements only coords have to be updated,
     * when appending is requested 
     */
    if (!is_append)
        construct_move_mouse_event(buf, n, *nx, *ny, false);
}


void handle_event(input_event* ie, char* buf, size_t n, bool is_append)
{
    
    /* Handles move events */
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

int run_template_injection_bak(qmp_connection* qc, FILE* f, bool* has_to_stop)
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];
    struct input_event ie;

    size_t bytes_read = 0;
    int nx, ny = 0;

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
            if (reset_hid_injection(qc, f, &tv_old, &nx, &ny) != 0)
                fprintf(stderr, "[HIDSIM] Error resetting HID injection");
        }

        bytes_read = fread(&ie, sizeof(ie), 1, f);
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

        /* Handles move events */
        if (ie.type == EV_REL)
        {
            if (ie.code == REL_X)
            {
                nx += ie.value;
            }
            if (ie.code == REL_Y)
            {
                ny += ie.value;
            }

            /* Throttles the sending of QMP commands */
            if (is_wait)
            {
                construct_move_mouse_command(buf, CMD_BUF_LEN, nx, ny, false);
                qmp_communicate(qc, buf, NULL);
                gettimeofday(&tv_last_qmp, NULL);
                is_wait = false;
            }
        }

        /* Handles click events */
        if (ie.type == EV_KEY)
        {
            if (ie.code < 0x100)
                //handle_key_event(&ie, buf, CMD_BUF_LEN);
                ;

            if (ie.code > 255 && ie.code < 0x120)
                //handle_btn_event(&ie, buf, CMD_BUF_LEN);
                ;
            qmp_communicate(qc, buf, NULL);
            gettimeofday(&tv_last_qmp, NULL);
        }

        gettimeofday(&tv_curr, NULL);
        timersub(&tv_curr, &tv_last_qmp, &tv_last_qmp_diff);

        tv_old.tv_sec = ie.time.tv_sec;
        tv_old.tv_usec = ie.time.tv_usec;
    }

    return 0;
}

int run_template_injection(qmp_connection* qc, FILE* f, bool* has_to_stop)
{   
    fprintf(stderr, "[HIDSIM] running template injection\n");
    /* Command buffer */
    char cmd[CMD_BUF_LEN];
    char buf[CMD_BUF_LEN];
    struct input_event ie_next, ie_cur;

    size_t nr = 0;
    //int new_x, new_y = 0;

    struct timeval tv_old;
    struct timeval tv_diff;

    
/* First event */
start:
    /* Prepares injection by initializing all variables */
    if (reset_hid_injection(qc, f, &tv_old, &new_x, &new_y) != 0)
    {
        fprintf(stderr, "[HIDSIM] Error resetting HID injection");
    }
    nr = fread(&ie_cur, sizeof(ie_cur), 1, f);

    long int sleep = ie_cur.time.tv_sec * 1000000 * ie_cur.time.tv_usec;
    usleep(sleep);

    while (!*has_to_stop)
    {
        /* Read follow up event */
        nr = fread(&ie_next, sizeof(ie_next), 1, f);
        
        timersub(&ie_next.time, &ie_cur.time, &tv_diff);
        sleep = tv_diff.tv_sec * 1000000 + tv_diff.tv_usec; 
        

        if ( sleep > 50)
        {
            /* Sends command buffer */ 
            handle_event(&ie_cur, buf, CMD_BUF_LEN, false);
            
            snprintf(cmd, CMD_BUF_LEN, "%s", "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [");
            snprintf(cmd + strlen(cmd), CMD_BUF_LEN - strlen(cmd), "%s", buf);
            snprintf(cmd + strlen(cmd), CMD_BUF_LEN - strlen(cmd), "%s", "] } }");
            
            qmp_communicate(qc, cmd, NULL);
            fprintf(stderr, "[HIDSIM] %s\n", cmd); 
            buf[0] = '\0';
            fprintf(stderr, "[HIDSIM] sleeping %ld\n", sleep); 
            usleep(sleep);
        }
        else 
        {
            /* Append to buffer */
            handle_event(&ie_cur, buf, CMD_BUF_LEN, true);
        }

        ie_cur = ie_next; 
        
        if (nr == 0)
        {
            goto start; 
        }

    }

    return 0;
}
void* run_key_injection(qmp_connection* qc, bool* has_to_stop){
    fprintf(stderr, "[HIDSIM] running key injection\n");

    /* Declares points in time */
    struct timeval t1;
    gettimeofday(&t1, NULL);

    /* Command buffer */
    char buf[CMD_BUF_LEN];
    int n = 2; 
    bool states_d[] = {true, true}; 
    bool states_u[] = {false, false};
    int keys[] = {KEY_LEFTSHIFT, KEY_A};
    int sign = 0x1;
    /* Loops, until stopped */
    while (!*has_to_stop)
    {
        //construct_key_tap(buf, CMD_BUF_LEN, keys, 2);
        construct_key_press(buf, CMD_BUF_LEN, keys, sign == 1?states_d:states_u, n);
        fprintf(stderr, "[HIDSIM] %s\n", buf);
        qmp_communicate(qc, buf, NULL); 
        fprintf(stderr, "[HIDSIM] Sent key press\n");
        //usleep(100);
        //construct_key_press(buf, CMD_BUF_LEN, keys, states_u, n);
        
        sign = ~sign + 1;
        usleep(100000);
    }
    return NULL;
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
        int fd = open(template_path, O_RDONLY);

        if(fd < 0){
            fprintf(stderr, "[HIDSIM] Error opening file %s\n", template_path);
            return NULL; 
        }

        /* Asks for aggressive readahead */
        if(posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL)!=0){
            fprintf(stderr, "[HIDSIM] Asking for aggressive readahead on FD %d failed. Continuing anyway...\n", fd);
        }

        FILE *f = fdopen(fd, "rb");

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
        //run_random_injection(&qc, has_to_stop);
        run_key_injection(&qc, has_to_stop);
    }

    /* Stop and clean up */
    qmp_close_conn(&qc);

    return NULL;
}
