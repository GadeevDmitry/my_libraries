#ifndef LOG_STATIC_H
#define LOG_STATIC_H

//================================================================================================================================

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "log.h"
#include "trace_static.h"

//================================================================================================================================

size_t LOG_TAB = 0;

//================================================================================================================================

#ifndef LOG_FILE
#define LOG_FILE "log.html"
#endif

//================================================================================================================================

static int         log_stream_open        ();
static void        log_stream_close       ();

static void        log_tab                ();
static void        log_print              (const char *log_buff, bool is_tab);

static inline void log_message            (const char *fmt, va_list ap);
static inline void log_tab_message        (const char *fmt, va_list ap);

static void        log_failure_environment(const char *const cur_file, const char *const cur_func, const int cur_line);

//================================================================================================================================

static FILE *LOG_STREAM            = nullptr;
static int   OPEN_CLOSE_LOG_STREAM = log_stream_open();

static int          DYNAMIC_MEMORY = 0;
static const size_t LOG_BUFF_SIZE  = 5000;

#endif // LOG_STATIC_H
