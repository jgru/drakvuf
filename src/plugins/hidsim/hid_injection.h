#ifndef HID_INJECTION_H
#define HID_INJECTION_H

#include <sys/un.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <stdbool.h>
#include "../private.h" //  PRINT_DEBUG
#define HEADER_LEN 0xC

#define RAND_INTERVAL 500
#define RAND_VARIANCE 800

/* Arguments passed to thread-functions */
typedef struct t_args
{
    /* Path to Unix domain socket */
    const char* socket_path;
    /* Path to template file */
    const char* template_path;
    /* Stop condition */
    bool* has_to_stop;
} t_args;

/* Inject random mouse movements */
void* run_random_injection(void* p);

/* Inject HID events from template file */
void* hid_inject(void* p);

#endif
