#include "execute.h"
#include "wrapper.h"
#include <stdio.h>


void *create_engine() {
    return call_init_engine();
}

void init(void *engine, char *host, int port, char *username, char *password, char* connId) {
  return call_init(engine, host, port, username, password, connId);
}

const char *execute(void *engine, char *cmd, char* connId) {
  return call_engine_execute(engine, cmd, connId);
}

void destroy(void *engine, char* connId) {
  return call_engine_destroy(engine, connId);
}

int getLastExecuteErrorCode(void *engine, char* connId) {
  return get_last_execute_error_code(engine, connId);
}