#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "../log.h"

MODULE_ID(9);

int main(int argc, char *argv[])
{
    int i = 0;

    log_init("test_log");
    SET_MODULE_NAME("module_test");
    
    SET_MODULE_LEVEL(0);
    i = 0;
    
    LOG_DEBUG("log level %d\n", i);
    LOG_INFO("log level %d\n", i);
    LOG_WARN("log level %d\n", i);
    LOG_ERROR("log level %d\n", i);
    LOG_EMERG("log level %d\n", i);
    LOG_EVENT("log level %d\n", i);    
    usleep(1000*1000);
    
    SET_MODULE_LEVEL(1);
    i = 1;
    
    LOG_DEBUG("log level %d\n", i);
    LOG_INFO("log level %d\n", i);
    LOG_WARN("log level %d\n", i);
    LOG_ERROR("log level %d\n", i);
    LOG_EMERG("log level %d\n", i);
    LOG_EVENT("log level %d\n", i);    
    usleep(1000*1000);
    
    SET_MODULE_LEVEL(4);
    i = 4;
    
    LOG_DEBUG("log level %d\n", i);
    LOG_INFO("log level %d\n", i);
    LOG_WARN("log level %d\n", i);
    LOG_ERROR("log level %d\n", i);
    LOG_EMERG("log level %d\n", i);
    LOG_EVENT("log level %d\n", i);    
    usleep(1000*1000);
    
    SET_MODULE_LEVEL(5);
    i = 5;
    
    LOG_DEBUG("log level %d\n", i);
    LOG_INFO("log level %d\n", i);
    LOG_WARN("log level %d\n", i);
    LOG_ERROR("log level %d\n", i);
    LOG_EMERG("log level %d\n", i);
    LOG_EVENT("log level %d\n", i);    
    usleep(1000*1000);
   
    log_destroy();
    
    printf("test log finished\n");

    return 0;
}


