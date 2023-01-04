#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "log_mgr.h"

#define MAX_LOG_LINES     10000
#define LOG_NAME_LEN      256
#define LOG_BUF_LEN       1024
#define DATA_TIME_STR_LEN 40

typedef struct log
{
    FILE *hnd;
    char name[LOG_NAME_LEN];
    unsigned char mode;
    int lines;
} log_t;

void get_date_time_string(char *str, int str_size)
{
    time_t curr_time = 0;
    struct tm *pt = NULL;

    assert(str);

    curr_time = time(NULL);
    pt = localtime(&curr_time);
    assert(pt);

    snprintf(str, str_size, "%04d%02d%02d%02d%02d%02d",
             pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday,
             pt->tm_hour, pt->tm_min, pt->tm_sec);
}

void add_one_trace(FILE *hnd, const char *trace)
{
    char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);

    // printf("%s %s", date_time, trace); // print to screen
    fprintf(hnd, "%s %s", date_time, trace);
}

FILE *create_log_file(const char *log_name)
{
    char date_time[DATA_TIME_STR_LEN];
    get_date_time_string(date_time, DATA_TIME_STR_LEN);

    char name[LOG_NAME_LEN];
    snprintf(name, LOG_NAME_LEN, "%s_%s.log", log_name, date_time);
    FILE *hnd = fopen(name, "wb+");
    if (!hnd)
    {
        return NULL;
    }

    add_one_trace(hnd, "log file created!!!\n");
    return hnd;
}

void *log_create(const char *log_name)
{
    log_t *log = (log_t *)malloc(sizeof(log_t));
    if (!log)
    {
        return NULL;
    }

    log->hnd = create_log_file(log_name);
    if (!log->hnd)
    {
        free(log);
        return NULL;
    }

    strncpy(log->name, log_name, sizeof(log->name));
    log->lines = 0;

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
        add_one_trace(tmp_log->hnd, "log file closed!!!\n");
        fclose(tmp_log->hnd);
        tmp_log->hnd = NULL;
    }

    free(tmp_log);
}

void log_add_one_trace(void *log, void *buf)
{
    log_t *tmp_log = (log_t *)log;
    if (!tmp_log)
    {
        return;
    }

    add_one_trace(tmp_log->hnd, buf);
    tmp_log->lines++;
    if (tmp_log->lines >= MAX_LOG_LINES)
    {
        FILE *hnd = create_log_file(tmp_log->name);
        if (hnd)
        {
            add_one_trace(tmp_log->hnd, "log file closed!!!\n");
            fclose(tmp_log->hnd);
            tmp_log->hnd = hnd;
            tmp_log->lines = 0;
        }
    }
}

void log_trace(void *log, const char *fmt, ...)
{
    char buf[LOG_BUF_LEN];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(log, buf);
}


log_mgr_t g_log_mgr;

void log_mgr_set_level(unsigned int mid, int level)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }
    
    g_log_mgr.level[mid] = level;

    return;
}

void log_mgr_set_name(unsigned int mid, const char *name)
{
    if (mid >= MIDS_NUM)
    {
        return;
    }
    
    strncpy(g_log_mgr.name[mid], name, MNAME_SIZE);
    g_log_mgr.name[mid][MNAME_SIZE - 1] = '\0';

    return;
}

int log_mgr_init(const char *log_name)
{
    memset(&g_log_mgr, 0, sizeof(log_mgr_t));
    g_log_mgr.log_hnd = log_create(log_name);
    if (g_log_mgr.log_hnd)
    {
        return 0;  // success
    }
    
    return -1; // failed
}

void log_mgr_destroy(void)
{
    if (g_log_mgr.log_hnd)
    {
        log_close(g_log_mgr.log_hnd);
        g_log_mgr.log_hnd = NULL;
    }
}

void log_mgr_trace(unsigned int mid, unsigned char level, const char *fmt, ...)
{
    if (level > g_log_mgr.level[mid])
    {
        return;
    }
    
    #define BUF_LEN           1024
    char buf[BUF_LEN];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_LEN, fmt, ap);
    va_end(ap);

    log_add_one_trace(g_log_mgr.log_hnd, buf);
}

