/** @file */
#ifndef LOG_DEF_H
#define LOG_DEF_H

#include "log_settings.h"

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

#if !defined(NLOG) && !defined(LOG_NTRACE)

#define $            _trace_upd (__FILE__, __PRETTY_FUNCTION__, __LINE__);  ///< обновление строки для следующего push
#define $i           _trace_push();                                         ///< push в стек trace
#define $o           _trace_pop ();                                         ///< pop из стека trace
#define trace_dump() _trace_dump()                                          ///< dump стека trace

#else

#define $
#define $i
#define $o
#define trace_dump()

#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_message(    fmt, ...) _log_message    (fmt, ##__VA_ARGS__)
#define log_tab_message(fmt, ...) _log_tab_message(fmt, ##__VA_ARGS__)
#else
#define log_message(    fmt, ...)
#define log_tab_message(fmt, ...)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR/WARNING
//--------------------------------------------------------------------------------------------------------------------------------

#if   !defined(NLOG) && !defined(LOG_NTRACE)

#define log_error(  fmt, ...) _log_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, true, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, true, fmt, ##__VA_ARGS__)

#elif !defined(NLOG) && defined(LOG_NTRACE)

#define log_error(  fmt, ...) _log_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, false, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, false, fmt, ##__VA_ARGS__)

#else

#define log_error(        fmt, ...)
#define log_oneline_error(fmt, ...)

#endif

#ifndef NLOG

#define log_error_message(fmt, ...) _log_error_message(fmt, ##__VA_ARGS__)
#define log_oneline_error(fmt, ...) _log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_warning_message(fmt, ...) _log_warning_message(fmt, ##__VA_ARGS__)
#define log_oneline_warning(fmt, ...) _log_oneline_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define log_error_message(fmt, ...)
#define log_oneline_error(fmt, ...)

#define log_warning_message(fmt, ...)
#define log_oneline_warning(fmt, ...)

#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_header(     fmt, ...)         _log_header     (fmt, ##__VA_ARGS__)
#define log_param_place(file, func, line) _log_param_place(file, func, line  )
#else
#define log_header(     fmt, ...)
#define log_param_place(file, func, line)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

#if !defined(NLOG) && !defined(LOG_NLEAK)

#define log_calloc(number, size) _log_calloc (number, size)
#define log_realloc(  ptr, size) _log_realloc(   ptr, size)
#define log_free(     ptr      ) _log_free   (   ptr      )

#else

#define log_calloc(number, size) calloc (number, size)
#define log_realloc(  ptr, size) realloc(   ptr, size)
#define log_free(     ptr      ) free   (   ptr      )

#endif

#endif //LOG_DEF_H