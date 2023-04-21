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
        fprintf(stderr, "ERROR: Can't open log file. All log-messages will disappear\n");
        return 0;
    }

    setvbuf(LOG_STREAM, nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n"
                        "\"%s\" OPENING IS OK\n\n", LOG_FILE);

    #ifndef LOG_NTRACE
    IS_TRACE_VALID = _trace_ctor(&TRACE);
    #endif

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void LOG_STREAM_CLOSE()
{
    assert(LOG_STREAM != nullptr);

    #ifndef LOG_NTRACE
    if (IS_TRACE_VALID) _trace_dtor(&TRACE);
    #endif

    fprintf(LOG_STREAM, "\n");

    if (DYNAMIC_MEMORY == 0) log_message(HTML_COLOR_LIME_GREEN "DYNAMIC_MEMORY = 0. \n" HTML_COLOR_CANCEL);
    else                     log_message(HTML_COLOR_DARK_RED   "DYNAMIC_MEMORY = %d.\n" HTML_COLOR_CANCEL, DYNAMIC_MEMORY);

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE SHELL
//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void _trace_push(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line)
{
    #ifdef LOG_NTRACE
    return;
    #else

    log_assert(cur_file != nullptr);
    log_assert(cur_func != nullptr);

    log_verify(IS_TRACE_VALID == true, (void) 0);

    IS_TRACE_VALID = _trace_push(&TRACE, cur_file, cur_func, cur_line);

    #endif //!LOG_NTRACE
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_pop()
{
    #ifdef LOG_NTRACE
    return;
    #else

    log_verify(IS_TRACE_VALID == true, (void) 0);

    _trace_pop(&TRACE);

    #endif //!LOG_NTRACE
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_dump()
{
    #ifdef LOG_NTRACE
    return;
    #else

    log_verify(IS_TRACE_VALID == true, (void) 0);

    _trace_dump(&TRACE);

    #endif //!LOG_NTRACE
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
    vsprintf(log_buff, fmt, ap);

    log_print(log_buff, false);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_tab_message(const char *fmt, ...)
{
    log_assert(fmt != nullptr);

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void _log_error(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line, const bool is_local_trace, const char *fmt, ...)
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

    #ifndef LOG_NTRACE
    if (is_local_trace) _trace_dump();
    #endif

    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

void _log_error_message(const char *fmt, ...)
{
    log_verify(fmt != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_message(HTML_COLOR_DARK_RED);

    va_list  ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    log_message(HTML_COLOR_CANCEL);
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void _log_warning(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line, const bool is_local_trace, const char *fmt, ...)
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

    #ifndef LOG_NTRACE
    if (is_local_trace) _trace_dump();
    #endif

    log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

void _log_warning_message(const char *fmt, ...)
{
    log_verify(fmt != nullptr, (void) 0);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    log_message(HTML_COLOR_DARK_ORANGE);

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    log_message(HTML_COLOR_CANCEL);
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
