/*
* from https://github.com/axenhook/log.git
*/


#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TO_NULL        0x00
#define LOG_TO_FILE        0x01
#define LOG_TO_SCREEN      0x02
#define LOG_TO_SCNFILE     (LOG_TO_FILE | LOG_TO_SCREEN)

#define MNAME_SIZE 32
#define MIDS_NUM 256

typedef struct log_mgr
{
    char name[MIDS_NUM][MNAME_SIZE];
    int level[MIDS_NUM];
    void *hnd;
} log_mgr_t;

extern log_mgr_t g_log;

static inline int log_get_level(unsigned int mid)
{
    return g_log.level[mid];
}

static inline char *log_get_name(unsigned int mid)
{
    return g_log.name[mid];
}

int log_init(const char *log_name, unsigned int mode, unsigned int max_lines);
void log_destroy(void);
void log_set_level(unsigned int mid, int level);
void log_set_name(unsigned int mid, const char *name);
void log_trace(unsigned int mid, unsigned char level, const char *fmt, ...);

#define MODULE_ID(mid) static unsigned int g_mid = (mid)

#define SET_MODULE_LEVEL(level) log_set_level(g_mid, level)
#define GET_MODULE_LEVEL() log_get_level(g_mid)
#define SET_MODULE_NAME(name) log_set_name(g_mid, name)
#define GET_MODULE_NAME() log_get_name(g_mid)

#if 1

#define LOG_DEBUG(fmt, ...)                                              \
    log_trace(g_mid, 5, "[DEBUG][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                               \
    log_trace(g_mid, 4, "[INFO ][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)                                               \
    log_trace(g_mid, 3, "[WARN ][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                              \
    log_trace(g_mid, 2, "[ERROR][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EMERG(fmt, ...)                                              \
    log_trace(g_mid, 1, "[EMERG][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EVENT(fmt, ...)                                              \
    log_trace(g_mid, 0, "[EVENT][%s:%s:%s:%d]: " fmt, GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else

#define LOG_DEBUG(fmt, ...)                                                                                        \
    log_trace(g_mid, 5, "[DEBUG][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                                                                         \
    log_trace(g_mid, 4, "[INFO ][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)                                                                                         \
    log_trace(g_mid, 3, "[WARN ][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                                                                        \
    log_trace(g_mid, 2, "[ERROR][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EMERG(fmt, ...)                                                                                        \
    log_trace(g_mid, 1, "[EMERG][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_EVENT(fmt, ...)                                                                                        \
    log_trace(g_mid, 0, "[EVENT][%lld][%s:%s:%s:%d]: " fmt, (unsigned long long)pthread_self(), GET_MODULE_NAME(), \
              __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif
