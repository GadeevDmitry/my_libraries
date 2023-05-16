/** @file */
#include "log_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// OPEN-CLOSE
//--------------------------------------------------------------------------------------------------------------------------------

static int LOG_STREAM_OPEN()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    if (LOG_STREAM == nullptr)
    {
        fprintf(stderr, BASH_COLOR_RED
                        "ERROR: Can't open log-file \"%s\". All log-messages will disappear.\n"
                        BASH_COLOR_WHITE, LOG_FILE);
        return 0;
    }

    setvbuf(LOG_STREAM, nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n"
                        "\"%s\" OPENING IS OK\n\n", LOG_FILE);

    #if !defined(NLOG) && !defined(LOG_NTRACE)
    IS_TRACE_VALID = _trace_ctor(&TRACE);
    #endif

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void LOG_STREAM_CLOSE()
{
    assert(LOG_STREAM != nullptr);

    #if !defined(NLOG) && !defined(LOG_NTRACE)
    if (IS_TRACE_VALID) _trace_dtor(&TRACE);
    #endif

    fprintf(LOG_STREAM, "\n");

    #if !defined(NLOG) && !defined(LOG_NLEAK)
    if (DYNAMIC_MEMORY == 0) log_ok_message   ("DYNAMIC_MEMORY = 0." , "\n");
    else                     log_error_message("DYNAMIC_MEMORY = %d.", "\n", DYNAMIC_MEMORY);
    #endif

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE SHELL
//--------------------------------------------------------------------------------------------------------------------------------

void log_trace_push()
{
    #if defined(NLOG) || defined(LOG_NTRACE)
    return;
    #else

    if (IS_TRACE_VALID == false) return;

    IS_TRACE_VALID = _trace_push(&TRACE);
    log_verify(IS_TRACE_VALID == true, (void) 0);

    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void log_trace_upd(const char *const file,
                   const char *const func,
                   const int         line)
{
    #if defined(NLOG) || defined(LOG_NTRACE)
    return;
    #else

    log_assert(file != nullptr);
    log_assert(func != nullptr);

    if (IS_TRACE_VALID == false) return; 

    IS_TRACE_VALID = _trace_front_upd(&TRACE, file, func, line);
    log_verify(IS_TRACE_VALID == true, (void) 0);

    #endif
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

void log_trace_pop()
{
    #if defined(NLOG) || defined(LOG_NTRACE)
    return;
    #else

    if (IS_TRACE_VALID == false) return;

    IS_TRACE_VALID = _trace_pop(&TRACE);
    log_verify(IS_TRACE_VALID == true, (void) 0);

    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_trace_dump()
{
    #if defined(NLOG) || defined(LOG_NTRACE)
    log_tab_message("NO TRACE SUPPORT\n");
    return;
    #else

    if (IS_TRACE_VALID == false) return;

    _trace_dump(&TRACE);

    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------
// log_print
//--------------------------------------------------------------------------------------------------------------------------------

static inline void log_tab()
{
    for (size_t i = 0; i < LOG_TAB; ++i) { fputc('\t', LOG_STREAM); }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_print(const char *log_buff, bool is_tab)
{
    log_assert(log_buff != nullptr);
    log_assert(log_buff[LOG_BUFF_SIZE - 1] == '\0');

    for (const char *log_pos = log_buff; *log_pos != '\0'; log_pos++)
    {
        if (*log_pos == '\n') { fputc('\n', LOG_STREAM); is_tab = true; }
        else
        {
            if (is_tab) { log_tab(); is_tab = false; }
            fputc(*log_pos, LOG_STREAM);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

void _log_message(const char *fmt, ...)
{
    log_verify(fmt != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);
   _log_message(fmt, ap);
}

static inline void _log_message(const char *fmt, va_list ap)
{
    log_assert(fmt != nullptr);
    log_assert(_OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf (log_buff, fmt, ap);
    log_print(log_buff, false);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_tab_message(const char *fmt, ...)
{
    log_verify(fmt != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);
   _log_tab_message(fmt, ap);
}

static inline void _log_tab_message(const char *fmt, va_list ap)
{
    log_assert(fmt != nullptr);
    log_assert(_OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf(log_buff, fmt, ap);

    log_print(log_buff, true);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

void _log_error(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line, const char *fmt, ...)
{
    log_assert(cur_file != nullptr);
    log_assert(cur_func != nullptr);
    log_verify(fmt      != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_RED "\n"
                                        "ERROR:\n");
    va_list  ap;
    va_start(ap, fmt);
   _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("====================\n");

    #if !defined(NLOG) && !defined(LOG_NTRACE)
    _log_trace_dump();
    #endif

    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_oneline_error(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line, const char *fmt, ...)
{
    log_assert(cur_file != nullptr);
    log_assert(cur_func != nullptr);
    log_verify(fmt      != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_RED "\n"
                                        "ERROR:\n");

    va_list  ap;
    va_start(ap, fmt);
   _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

void _log_warning(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line, const char *fmt, ...)
{
    log_assert(cur_file != nullptr);
    log_assert(cur_func != nullptr);
    log_verify(fmt      != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                                           "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
   _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("====================\n");

    #if !defined(NLOG) && !defined(LOG_NTRACE)
    _log_trace_dump();
    #endif

    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_oneline_warning(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line, const char *fmt, ...)
{
    log_assert(cur_file != nullptr);
    log_assert(cur_func != nullptr);
    log_verify(fmt      != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                                           "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

void _log_header(const char *fmt, ...)
{
    log_verify(fmt != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    fprintf(LOG_STREAM, "<h2>\n");

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    fprintf(LOG_STREAM, "</h2>\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_param_place(const char *const file,
                      const char *const func,
                      const int         line)
{
    log_verify(file != nullptr, (void) 0);
    log_verify(func != nullptr, (void) 0);

    log_tab_message("    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n", file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

void *_log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if   (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *_log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr == nullptr && size == 0)                     return ret;
    if      (ptr == nullptr             ) { ++DYNAMIC_MEMORY; return ret; }
    else if (                  size == 0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}
