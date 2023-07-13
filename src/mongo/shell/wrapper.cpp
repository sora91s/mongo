#include "wrapper.h"
#include "m_engine.h"

std::mutex mtx;
bool isGlobalInit = false;

void call_init(void *engine, char *host, int port, char *username, char *password, char* connId) {
    if (!isGlobalInit)
    {
        mtx.lock();
        if (!isGlobalInit)
        {
            call_init_global(engine);
            isGlobalInit = true;
        }
        mtx.unlock();
    }
    int initRes = static_cast<mongo::MEngine*>(engine)->init(host, port, username, password, connId);
    static_cast<mongo::MEngine*>(engine)->errorCodes[connId] = initRes;
}

void *call_init_engine() {
    mongo::MEngine *res = new mongo::MEngine();
    return res;
}

void call_init_global(void *engine) {
    static_cast<mongo::MEngine*>(engine)->initGlobal();
}


const char *call_engine_execute(void *engine, char *cmd, char* connId) {
    return static_cast<mongo::MEngine*>(engine)->exec(cmd, connId);
}

void call_engine_destroy(void* engine, char* connId) {
    return static_cast<mongo::MEngine*>(engine)->destroy(connId);
}

int get_last_execute_error_code(void* engine, char* connId) {
    return static_cast<mongo::MEngine*>(engine)->errorCodes[connId];
}
