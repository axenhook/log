/*
* from https://github.com/axenhook/log_simple.git
*/

#include <assert.h>
#include <stdio.h>
#include <signal.h>

#include "../log.h"
#include "threads_group.h"


extern void *log_create(const char *log_name, unsigned int mode, unsigned int max_lines);
extern void  log_trace_internal(void *log, const char *format, ...);
extern void  log_close(void *log);

#if 1

#define log_trace(log_hnd, fmt, ...)        \
    log_trace_internal(log_hnd, "[%s:%s:%d]: "fmt, \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else

#define log_trace(log_hnd, fmt, ...)        \
    log_trace_internal(log_hnd, "[%lld][%s:%s:%d]: "fmt, (unsigned long long)pthread_self(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif
       
void test_log(void *log)
{
    for (unsigned int i = 0; i < 1000; i++) 
    {
        log_trace(log, "Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d\n", i);
    }
}

void basic_test(void)
{
    printf("basic test start\n");

    void *log1 = log_create("test_log_screen", LOG_TO_SCREEN, 0);
    assert(log1);
    
    void *log2 = log_create("test_log_file", LOG_TO_FILE, 789);
    assert(log2);

    void *log3 = log_create("test_log_scnfile", LOG_TO_FILE | LOG_TO_SCREEN, 567);
    assert(log3);

    test_log(log1);
    test_log(log2);
    test_log(log3);

    log_close(log1);
    log_close(log2);
    log_close(log3);
    
    printf("basic test finished\n");
}


static int g_exit_flag = 0;

void trigger_stop(int param)
{
    g_exit_flag = 1;
}

void *thread_test(void *arg, unsigned int thread_id)
{
    unsigned int i = 0;

    while (1)
    {
        if (g_exit_flag)
        {
            break;
        }

        log_trace(arg, "Test very1 very2 very3 very4 very5 very6 very7 very8 very9 very10 very11 very12 very13 very14 very15 very16 long %d\n", i++);

        // usleep(100 * 1000);
    }

    printf("The thread stopped. [thread_id: %u, thread: %lu]\n",
           thread_id, os_get_thread_id());

    return NULL;
}

void multi_thread_test(void)
{
    g_exit_flag = 0;

    signal(SIGINT, trigger_stop);
    // signal(SIGKILL, trigger_stop);
    signal(SIGTERM, trigger_stop);

    void *log = log_create("test_multi_threads", LOG_TO_FILE | LOG_TO_SCREEN, 12345);
    assert(log);

    void *group = threads_group_start(20, thread_test, log, "test_log");
	os_sleep_ms(3*1000);
	trigger_stop(1);
    threads_group_stop(group, 1);

    log_close(log);
}

int main(int argc, char *argv[])
{
    printf("test log start\n");
    basic_test();
    multi_thread_test();
    printf("test log finished\n");

	system("pause");

    return 0;
}


