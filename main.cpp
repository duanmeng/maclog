#include "log.h"

void* foo(void *) 
{
    DEBUG("sucker debug %d %s %f", 1, "hello", 3.14);
    INFO("sucker info %d %s %f", 1, "hello", 3.14);
    WARNING("sucker warning %d %s %f", 1, "hello", 3.14);
    ERROR("sucker error %d %s %f", 1, "hello", 3.14);
    FATAL("sucker fatal %d %s %f", 1, "hello", 3.14);
}

int main() 
{
    //logging::Log::init_log("/home/duanmeng/tmp/log.%Y-%m-%d-%H-%M.%S", logging::Log::DEBUG);
    logging::Log::init_log(NULL, logging::Log::DEBUG);
    pthread_t pids[5];
    for (int i = 0; i != 5; ++i) {
        pthread_create(pids + i , NULL, foo, NULL);
    }
    for (int i = 0; i != 5; ++i) {
        pthread_join(pids[i], NULL);
    }
    return 0;
}
