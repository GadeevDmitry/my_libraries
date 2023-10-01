#include "log_static.h"

//================================================================================================================================

static int log_stream_open()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    if (LOG_STREAM == nullptr)
    {
        STDERR_ERROR_MESSAGE("ERROR: Can't open log-file \"%s\". All log-messages will disappear.\n", LOG_FILE);
        return 0;
    }

    setvbuf(LOG_STREAM, nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n" "\"%s\" OPENING IS OK\n\n", LOG_FILE);

    DYNAMIC_MEMORY -= 1;
    trace_ctor();
    atexit(trace_dtor);

    atexit (log_stream_close);
    return 1;
}

static void log_stream_close()
{
    assert(LOG_STREAM != nullptr);

    fprintf(LOG_STREAM, "\n");

    if (DYNAMIC_MEMORY == 0) LOG_OK_MESSAGE   ("DYNAMIC_MEMORY = 0." , "\n\n");
    else                     LOG_ERROR_MESSAGE("DYNAMIC_MEMORY = %d.", "\n\n", DYNAMIC_MEMORY);

    size_t trace_size = trace_get_size();
    if (trace_size == 0) LOG_OK_MESSAGE   ("STACK TRACE SIZE = 0."  , "\n\n");
    else                 LOG_ERROR_MESSAGE("STACK TRACE SIZE = %lu.", "\n\n", trace_size);

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void log_tab()
{
    for (size_t i = 0; i < LOG_TAB; ++i) { fputc('\t', LOG_STREAM); }
}

static void log_print(const char *log_buff, bool is_tab)
{
    assert(log_buff != nullptr);

    for (const char *log_pos = log_buff; *log_pos != '\0'; log_pos++)
    {
        if (*log_pos == '\n')
        {
            fputc('\n', LOG_STREAM);
            is_tab = true;
        }
        else
        {
            if (is_tab) { log_tab(); is_tab = false; }
            fputc(*log_pos, LOG_STREAM);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_message(const char *fmt, ...)
{
    assert(fmt != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);
    log_message(fmt, ap);
    va_end(ap);
}

static inline void log_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);
    assert(OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsnprintf(log_buff, LOG_BUFF_SIZE - 1, fmt, ap);
    log_print(log_buff, false);
}

void log_tab_message(const char *fmt, ...)
{
    assert(fmt != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);
}

static inline void log_tab_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);
    assert(OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsnprintf(log_buff, LOG_BUFF_SIZE - 1, fmt, ap);
    log_print(log_buff, true);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_failure_environment(const char *const cur_file, const char *const cur_func, const int cur_line)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);
    assert(OPEN_CLOSE_LOG_STREAM != 0);

    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message(ITALIC_LOG_SEP);

    trace_dump();
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_error(const char *const cur_file, const char *const cur_func, const int cur_line,
               const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);
    assert(fmt      != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_RED BOLD_LOG_SEP "ERROR:\n");

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);

    log_tab_message(ITALIC_LOG_SEP);
    log_failure_environment(cur_file, cur_func, cur_line);
    log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
}

void log_oneline_error(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line, const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);
    assert(fmt      != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_RED BOLD_LOG_SEP "ERROR:\n");

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);

    log_tab_message(ITALIC_LOG_SEP);
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
}

void log_warning(const char *const cur_file, const char *const cur_func, const int cur_line,
                 const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);
    assert(fmt      != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_ORANGE BOLD_LOG_SEP "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);

    log_tab_message(ITALIC_LOG_SEP);
    log_failure_environment(cur_file, cur_func, cur_line);
    log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
}

void log_oneline_warning(const char *const cur_file, const char *const cur_func, const int cur_line,
                         const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);
    assert(fmt      != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_ORANGE BOLD_LOG_SEP "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);

    log_tab_message(ITALIC_LOG_SEP);
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_assertion_failed(const char *const cur_file, const char *const cur_func, const int cur_line,
                          const char *report_message)
{
    assert(cur_file       != nullptr);
    assert(cur_func       != nullptr);
    assert(report_message != nullptr);

    if (OPEN_CLOSE_LOG_STREAM != 0)
    {
        log_tab_message(HTML_COLOR_DARK_RED BOLD_LOG_SEP "ASSERTION FAILED\n" "%s\n", report_message);
        log_tab_message(ITALIC_LOG_SEP);
        log_failure_environment(cur_file, cur_func, cur_line);
        log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
    }

    STDERR_ERROR_MESSAGE("ASSERTION FAILED: check \"%s\"\n", LOG_FILE);
    abort();
}

void log_verification_failed(const char *const cur_file, const char *const cur_func, const int cur_line,
                             const char *report_message)
{
    assert(cur_file       != nullptr);
    assert(cur_func       != nullptr);
    assert(report_message != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    log_tab_message(HTML_COLOR_DARK_RED BOLD_LOG_SEP "VERIFICATION FAILED\n" "%s\n", report_message);
    log_tab_message(ITALIC_LOG_SEP);
    log_failure_environment(cur_file, cur_func, cur_line);
    log_tab_message(BOLD_LOG_SEP HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_header(const char *fmt, ...)
{
    assert(fmt != nullptr);

    if (OPEN_CLOSE_LOG_STREAM == 0) return;

    fprintf(LOG_STREAM, "<h2>\n");

    va_list ap;
    va_start(ap, fmt);
    log_tab_message(fmt, ap);
    va_end(ap);

    fprintf(LOG_STREAM, "</h2>\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_param_place(const char *const file, const char *const func, const int line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("FILE    : %s\n"
                    "FUNCTION: %s\n"
                    "LINE    : %d\n", file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

void *log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr == nullptr && size == 0)                     return ret;
    if      (ptr == nullptr             ) { ++DYNAMIC_MEMORY; return ret; }
    else if (                  size == 0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

void *log_recalloc(void *ptr, size_t old_size, size_t new_size, const bool is_nleak)
{
    void *ret = realloc(ptr, new_size);

    if (ptr == nullptr && new_size == 0)                                        return ret;
    if (ptr == nullptr)                  { if (!is_nleak) { ++DYNAMIC_MEMORY; } return ret; }
    if (                  new_size == 0) { if (!is_nleak) { --DYNAMIC_MEMORY; } return ret; }

    if (new_size > old_size) memset((char *) ret + old_size, 0, new_size - old_size);

    return ret;
}

void log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}
