#ifndef QMP_H
#define QMP_H

#include <sys/un.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <stdbool.h>

#define EXEC_QMP_CAPABILITIES "{ \"execute\": \"qmp_capabilities\" }"
#define QMP_SUCCESS "{\"return\": {}}\r\n"

#define BUF_SIZE 0x2000

typedef void (*event_cb)(json_object* event);

/* Connection to a QMP server */
typedef struct qmp_connection
{
    int fd;
    struct sockaddr_un sa;
    json_object* capabilities;
    char buf[BUF_SIZE];
    bool is_connected;
} qmp_connection;

/* Takes a caller allocated QMPConnection strcut and initializes connection to unix domain socket */
int qmp_init_conn(qmp_connection* qc,  char const* path);

/* Takes a caller allocated QMPConnection strcut and initializes connection to TCP socket */
int init_conn_tcp_sock(qmp_connection* qc, char const* path);

/* Sends the given data to the QMPConnection */
int qmp_communicate_json(qmp_connection* qc, json_object* in, json_object** out);

/* Sends the given data to the QMPConnection */
int qmp_communicate(qmp_connection* qc, const char* in, char** out);

/* Clean-up */
int qmp_close_conn(qmp_connection* qc);

#endif
