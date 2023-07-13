#ifndef WRAPPER_H_
#define WRAPPER_H_

#ifdef __cplusplus
extern "C" 
{
#endif
void *call_init_engine();
void call_init_global(void*);
void call_init(void*, char *host, int port, char *username, char *password, char* connId);
const char *call_engine_execute(void*, char*, char* connId);
void call_engine_destroy(void*, char* connId);
int get_last_execute_error_code(void*, char* connId);

#ifdef __cplusplus
}
#endif

#endif // WRAPPER_H_
