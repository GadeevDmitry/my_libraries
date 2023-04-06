#ifndef LOG_DEF_H
#define LOG_DEF_H

#include "log_settings.h"

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define trace_dump() _trace_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
#define trace_dump()
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_message(    fmt, ...) _log_message    (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_tab_message(fmt, ...) _log_tab_message(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_message(    fmt, ...)
#define log_tab_message(fmt, ...)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_error(        fmt, ...) _log_error        (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error_message(fmt, ...) _log_error_message(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_oneline_error(fmt, ...) _log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_error(        fmt, ...)
#define log_error_message(fmt, ...)
#define log_oneline_error(fmt, ...)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_warning(        fmt, ...) _log_warning        (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning_message(fmt, ...) _log_warning_message(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_oneline_warning(fmt, ...) _log_oneline_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_warning(        fmt, ...)
#define log_warning_message(fmt, ...)
#define log_oneline_warning(fmt, ...)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_header(     fmt, ...)         _log_header     (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_param_place(file, func, line) _log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__, file, func, line  )
#else
#define log_header(     fmt, ...)
#define log_param_place(file, func, line)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_calloc(number, size) _log_calloc (__FILE__, __PRETTY_FUNCTION__, __LINE__, number, size)
#define log_realloc(  ptr, size) _log_realloc(__FILE__, __PRETTY_FUNCTION__, __LINE__,    ptr, size)
#define log_free(     ptr      ) _log_free   (__FILE__, __PRETTY_FUNCTION__, __LINE__,    ptr      )
#else
#define log_calloc(number, size) calloc (number, size)
#define log_realloc(  ptr, size) realloc(   ptr, size)
#define log_free(     ptr      ) free   (   ptr      )
#endif

#endif //LOG_DEF_H