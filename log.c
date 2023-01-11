/*
* from https://github.com/axenhook/log.git
*/

#ifdef WIN32  // windows
#define _CRT_SECURE_NO_WARNINGS  // mask fopen_s like warning
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "log.h"

//============================================================================
// basic log functions
//============================================================================

#define LOG_NAME_LEN      256
#define LOG_BUF_LEN       1024
#define DATA_TIME_STR_LEN 40

#ifdef WIN32  // windows

#include <windows.h>

#define atomic_inc(x)           InterlockedIncrement(x)

typedef CRITICAL_SECTION        os_mutex_t;
#define os_mutex_init(mutex)    InitializeCriticalSection(mutex)
#define os_mutex_lock(mutex)    EnterCriticalSection(mutex)
#define os_mutex_unlock(mutex)  LeaveCriticalSection(mutex)
#define os_mutex_destroy(mutex) DeleteCriticalSection(mutex)

#else  // linux user space

#include <pthread.h>

#define atomic_inc(x)           __sync_fetch_and_add(x, 1)

typedef pthread_mutex_t         os_mutex_t;
#define os_mutex_init(mutex)    pthread_mutex_init(mutex, NULL)
#define os_mutex_lock(mutex)    pthread_mutex_lock(mutex)
#define os_mutex_unlock(mutex)  pthread_mutex_unlock(mutex)
#define os_mutex_destroy(mutex) pthread_mutex_destroy(mutex)

#endif

typedef struct log
{
    FILE        *hnd;
    char         name[LOG_NAME_LEN];
    unsigned int mode;
    unsigned int max_lines;
    os_mutex_t   mutex;

    unsigned int lines;
    unsigned int count; // number of log file changes
} log_t;

void get_date_time_string(char *str, unsigned int str_size, char *fmt)
{
    time_t curr_time;
    struct tm pt;

    assert(str);

    curr_time = time(NULL);
#ifdef WIN32
    localtime_s(&pt, &curr_time);
#else
    localtime_r(&curr_time, &pt);
#endif

    snprintf(str, str_size, fmt,
             pt.tm_year + 1900, pt.tm_mon + 1, pt.tm_mday,
             pt.tm_hour, pt.tm_min, pt.tm_sec);
}

void add_one_trace(log_t *log, const char *trace)
{
    char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN, "%04d%02d%02d %02d:%02d:%02d");

    if (log->mode & LOG_TO_FILE)
    {
        fprintf(log->hnd, "%s %s", date_time, trace);
        //log->lines++;
        atomic_inc(&log->lines);
    }

    if (log->mode & LOG_TO_SCREEN)
    {
        printf("%s %s", date_time, trace);
    }
}

void add_only_file_trace(FILE *hnd, const char *trace)
{
    char date_time[DATA_TIME_STR_LEN];

    get_date_time_string(date_time, DATA_TIME_STR_LEN, "%04d%02d%02d %02d:%02d:%02d");
    fprintf(hnd, "%s %s", date_time, trace);
}

FILE *create_log_file(const char *log_name, unsigned int count)
{
    char date_time[DATA_TIME_STR_LEN];
    char name[LOG_NAME_LEN];

    get_date_time_string(date_time, DATA_TIME_STR_LEN, "%04d%02d%02d_%02d%02d%02d");
    snprintf(name, LOG_NAME_LEN, "%s_%s_%04u.log", log_name, date_time, count);
    FILE *hnd = fopen(name, "wb+");
    if (!hnd)
    {
        return NULL;
    }

    add_only_file_trace(hnd, "log file created!!!\n");

    return hnd;
}

void *log_create(const char *log_name, unsigned int mode, unsigned int max_lines)
{
    log_t *log = (log_t *)malloc(sizeof(log_t));
    if (!log)
    {
        return NULL;
    }

    memset(log, 0, sizeof(log_t));
	
    if (mode & LOG_TO_FILE)
    {
        log->hnd = create_log_file(log_name, log->count);
        if (!log->hnd)
        {
            free(log);
            return NULL;
        }

        strncpy(log->name, log_name, sizeof(log->name));
    }

    os_mutex_init(&log->mutex);
    log->mode = mode;
    log->max_lines = max_lines;
    log->lines = 0;
    log->count++;

    return log;
}

void log_close(void *log)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    if (tmp_log->hnd)
    {
        add_only_file_trace(tmp_log->hnd, "log file closed!!!\n");
        fclose(tmp_log->hnd);
        tmp_log->hnd = NULL;
    }
    
    os_mutex_destroy(&tmp_log->mutex);

    free(tmp_log);
}

void log_set_max_lines(void *log, unsigned int max_lines)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    tmp_log->max_lines = max_lines;
}

void log_add_one_trace(void *log, void *buf)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    add_one_trace(tmp_log, buf);
    if ((tmp_log->lines >= tmp_log->max_lines) && (tmp_log->mode & LOG_TO_FILE))
    {
        os_mutex_lock(&tmp_log->mutex);
        
        if (tmp_log->lines >= tmp_log->max_lines) // check again
        {
            FILE *hnd = create_log_file(tmp_log->name, tmp_log->count);
            if (hnd)
            {
                add_only_file_trace(tmp_log->hnd, "log file closed!!!\n");
    
                fclose(tmp_log->hnd);
                tmp_log->hnd = hnd;
    
                tmp_log->lines = 0;
                tmp_log->count++;
            }
        }
        
        os_mutex_unlock(&tmp_log->mutex);
    }
}

#if 0

void log_trace(void *log, const char *fmt, ...)
{
    char buf[LOG_BUF_LEN];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(log, buf);
}

#endif

//============================================================================
// log module manager functions
//============================================================================

#define MNAME_SIZE 32
#define MIDS_NUM 256

typedef struct log_mgr
{
    char name[MIDS_NUM][MNAME_SIZE];
    int level[MIDS_NUM];
    void *hnd;
} log_mgr_t;

log_mgr_t g_log;

int log_get_level(unsigned int mid)
{
    return g_log.level[mid];
}

char *log_get_name(unsigned int mid)
{
    return g_log.name[mid];
}

void log_set_level(unsigned int mid, int level)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }

    g_log.level[mid] = level;

    return;
}

void log_set_name(unsigned int mid, const char *name)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }

    strncpy(g_log.name[mid], name, MNAME_SIZE);
    g_log.name[mid][MNAME_SIZE - 1] = '\0';

    return;
}

int log_init(const char *log_name, unsigned int mode, unsigned int max_lines)
{
    memset(&g_log, 0, sizeof(log_mgr_t));
    g_log.hnd = log_create(log_name, mode, max_lines);
    if (g_log.hnd)
    {
        return 0; // success
    }

    return -1; // failed
}

void log_destroy(void)
{
    if (g_log.hnd)
    {
        log_close(g_log.hnd);
        g_log.hnd = NULL;
    }
}

void log_trace(unsigned int mid, unsigned char level, const char *fmt, ...)
{
    if (level > g_log.level[mid])
    {
        return;
    }

    char buf[LOG_BUF_LEN];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(g_log.hnd, buf);
}
