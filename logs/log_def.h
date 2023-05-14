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

#elif !defined(NLOG) &&  defined(LOG_NTRACE)

#define log_error(  fmt, ...) _log_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, false, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, false, fmt, ##__VA_ARGS__)

#else // defined(NLOG)

#define log_error(  fmt, ...)
#define log_warning(fmt, ...)

#endif

#ifndef NLOG
#define log_oneline_error(  fmt, ...) _log_oneline_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_oneline_warning(fmt, ...) _log_oneline_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_oneline_error(  fmt, ...)
#define log_oneline_warning(fmt, ...)
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef NLOG
#define log_ok_message(     fmt, end, ...) log_message(HTML_COLOR_LIME_GREEN  fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но LIME_GREEN  цветом
#define log_error_message(  fmt, end, ...) log_message(HTML_COLOR_DARK_RED    fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но DARK_RED    цветом
#define log_warning_message(fmt, end, ...) log_message(HTML_COLOR_DARK_ORANGE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но DARK_ORANGE цветом
#define log_service_message(fmt, end, ...) log_message(HTML_COLOR_MEDIUM_BLUE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но MEDIUM_BLUE цветом
#define log_default_message(fmt, end, ...) log_message(HTML_COLOR_BLACK       fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но BLACK       цветом
#define log_poison_message( fmt, end, ...) log_message(HTML_COLOR_POISON      fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_message, но OLIVE       цветом

#define log_tab_ok_message(     fmt, end, ...) log_tab_message(HTML_COLOR_LIME_GREEN  fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но LIME_GREEN  цветом
#define log_tab_error_message(  fmt, end, ...) log_tab_message(HTML_COLOR_DARK_RED    fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но DARK_RED    цветом
#define log_tab_warning_message(fmt, end, ...) log_tab_message(HTML_COLOR_DARK_ORANGE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но DARK_ORANGE цветом
#define log_tab_service_message(fmt, end, ...) log_tab_message(HTML_COLOR_MEDIUM_BLUE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но MEDIUM_BLUE цветом
#define log_tab_default_message(fmt, end, ...) log_tab_message(HTML_COLOR_BLACK       fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но BLACK       цветом
#define log_tab_poison_message( fmt, end, ...) log_tab_message(HTML_COLOR_POISON      fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__) ///< ~ log_tab_message, но OLIVE       цветом
#else // NLOG
#define log_ok_message(     fmt, end, ...)
#define log_error_message(  fmt, end, ...)
#define log_warning_message(fmt, end, ...)
#define log_service_message(fmt, end, ...)
#define log_default_message(fmt, end, ...)
#define log_poison_message( fmt, end, ...)

#define log_tab_ok_message(     fmt, end, ...)
#define log_tab_error_message(  fmt, end, ...)
#define log_tab_warning_message(fmt, end, ...)
#define log_tab_service_message(fmt, end, ...)
#define log_tab_default_message(fmt, end, ...)
#define log_tab_poison_message( fmt, end, ...)
#endif

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

//--------------------------------------------------------------------------------------------------------------------------------
// BASH
//--------------------------------------------------------------------------------------------------------------------------------

#define fprintf_ok_message(   stream, fmt, ...) fprintf(stream, BASH_COLOR_GREEN fmt BASH_COLOR_WHITE, ##__VA_ARGS__)
#define fprintf_error_message(stream, fmt, ...) fprintf(stream, BASH_COLOR_RED   fmt BASH_COLOR_WHITE, ##__VA_ARGS__)

#define printf_ok_message(   fmt, ...) fprintf_ok_message   (stdout, fmt, ##__VA_ARGS__)
#define printf_error_message(fmt, ...) fprintf_error_message(stdout, fmt, ##__VA_ARGS__)

#endif //LOG_DEF_H
