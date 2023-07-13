#ifndef HELLO_H_
#define HELLO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void *create_engine();
extern void init(void *engine, char* host, int port, char* username, char* password, char* connId);
const extern char *execute(void *engine, char *cmd, char* connId);
extern void destroy(void *engine, char* connId);
extern int getLastExecuteErrorCode(void *engine, char* connId);

#ifdef __cplusplus
}
#endif

#endif // HELLO_H_
