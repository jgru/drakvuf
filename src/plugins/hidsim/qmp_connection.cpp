#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "qmp_connection.h"
#include "plugins/plugins_ex.h"

/* Takes a caller allocated QMPConnection strcut and initializes connection */
int qmp_init_conn(qmp_connection *qc,  const char *path){
  int ret = -1;
  
  /* Creates a socket */
  qc->fd = socket(AF_UNIX, SOCK_STREAM, 0);
  
  if (qc->fd == -1) {
      fprintf(stderr,"socket: %s\n", strerror(errno));
      return qc->fd;
  }
  qc->sa.sun_family = AF_UNIX; 

  /* Check, if path to domain is too long */
  if((strlen(path) + 1) > sizeof(qc->sa.sun_path)){
      fprintf(stderr,"path to domain socket exceeds %ld characters\n", sizeof(qc->sa.sun_path) );
      return qc->fd;
  } 

  /* Sets socket path */
  strcpy(qc->sa.sun_path, path);

  /* Initializes connection */
  ret = connect(qc->fd, (struct sockaddr*)&qc->sa, sizeof(qc->sa));
  
  if (ret == -1) {
    fprintf(stderr,"connect: %s - %s\n", strerror(errno), qc->sa.sun_path);
    return -1;
  }
  
    /* Reads the capabilities, which are sent by the qmp-server */
    ret = read(qc->fd, qc->buf, sizeof(qc->buf));

    /* Stores capabilities for later use */
    qc->capabilities = json_tokener_parse(qc->buf);
    memset(qc->buf, 0, sizeof(qc->buf));

    /* Exits capabilities negotiation mode */
    ret = write(qc->fd, EXEC_QMP_CAPABILITIES, strlen(EXEC_QMP_CAPABILITIES));

    if (ret == -1)
    {
        fprintf(stderr, "write to %s - %s\n", qc->sa.sun_path, strerror(errno));
        return -1;
    }
    /* Check return value */
    ret = read(qc->fd, qc->buf, sizeof(qc->buf));

    if (ret == -1)
    {
        fprintf(stderr, "read from %s - %s\n", qc->sa.sun_path, strerror(errno));
        return -1;
    }

    ret = strcmp(qc->buf, QMP_SUCCESS);
    if (ret != 0)
    {
        fprintf(stderr, "Capability negotiation failed, msg: %s\n", qc->buf);
        return -1;
    }

    qc->is_connected = true;
    PRINT_DEBUG("[QMP_CONNECTION] Connected to Unix Domain Socket %s.\n", path);

    return 0;
}

/* Sends the given data to the QMPConnection and reads the response */
int qmp_communicate_json(qmp_connection* qc, struct json_object* in, struct json_object** out)
{

    const char* in_str = json_object_to_json_string(in);
    char* out_str = NULL;
    qmp_communicate(qc, in_str, &out_str);
    *out = json_tokener_parse(out_str);

    if (out_str)
        free(out_str);

    return 0;
}

/* Sends the given data to the QMPConnection and reads the response */
int qmp_communicate(qmp_connection* qc, const char* in, char** out){

    int ret = write(qc->fd, in, strlen(in));

    if (ret == -1)
    {
        fprintf(stderr, "write to %s - %s\n", qc->sa.sun_path, strerror(errno));
        return ret;
    }

    /* Reads the response */
    ret = read(qc->fd, qc->buf, sizeof(qc->buf));

    if (ret == -1)
    {
        fprintf(stderr, "read from %s - %s\n", qc->sa.sun_path, strerror(errno));
        return ret;
    }
    if (out)
        *out = strdup(qc->buf);

    memset(qc->buf, 0, sizeof(qc->buf));
    
    return 0; 
}

/* Clean-up */
int qmp_close_conn(qmp_connection* qc){
  close(qc->fd);

  if(qc->capabilities)
    free(qc->capabilities);

  return 0;
}
