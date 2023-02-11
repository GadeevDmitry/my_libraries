/** @file */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "log_static.h"

//================================================================================================================================
// GLOBAL
//================================================================================================================================

size_t LOG_TAB = 0; ///< количество табов, необходимое отступить перед записью в лог

//================================================================================================================================
// FUNCTION
//================================================================================================================================

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

    setvbuf(LOG_STREAM,   nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n""\"%s\" OPENING IS OK\n\n", LOG_FILE);

    #ifndef NTRACE
    trace_ctor();
    #endif

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void LOG_STREAM_CLOSE()
{
    assert(LOG_STREAM != nullptr);

    #ifndef NTRACE
    trace_dtor();
    #endif

    fprintf(LOG_STREAM, "\n");

    if (DYNAMIC_MEMORY == 0) _stat_log_message(HTML_COLOR_LIME_GREEN "DYNAMIC_MEMORY = 0. \n" HTML_COLOR_CANCEL                );
    else                     _stat_log_message(HTML_COLOR_DARK_RED   "DYNAMIC_MEMORY = %d.\n" HTML_COLOR_CANCEL, DYNAMIC_MEMORY);

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);

}

//--------------------------------------------------------------------------------------------------------------------------------
// source_pos
//--------------------------------------------------------------------------------------------------------------------------------

static void source_pos_ctor(source_pos *const src_pos,  const char *const file,
                                                        const char *const func,
                                                        const int         line)
{
    assert(src_pos != nullptr);
    assert(file    != nullptr);
    assert(func    != nullptr);

    src_pos->file = file;
    src_pos->func = func;
    src_pos->line = line;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void source_pos_dump(const source_pos *const src_pos)
{
    assert(src_pos       != nullptr);
    assert(src_pos->file != nullptr);
    assert(src_pos->func != nullptr);

    _log_param_place(src_pos->file, src_pos->func, src_pos->line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

static bool trace_ctor()
{
    #ifdef NTRACE
    return true;
    #endif

    TRACE.data = (source_pos *) _log_calloc(DEFAULT_TRACE_CAPACITY, sizeof(source_pos));
    if (TRACE.data == nullptr)
    {
        _stat_log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, "can't allocate memory for TRACE.data");
        abort();
        return false;
    }

    TRACE.size     = 0;
    TRACE.capacity = DEFAULT_TRACE_CAPACITY;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_dtor()
{
    #ifdef NTRACE
    return;
    #endif

    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    _log_free(TRACE.data);
    TRACE.size       = 0;
    TRACE.capacity   = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool trace_push(const char *const file,
                const char *const func,
                const int         line)
{
    #ifdef NTRACE
    return true;
    #endif

    assert(file != nullptr);
    assert(func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return true;

    if (!trace_resize()) return false;
    source_pos_ctor(TRACE.data + TRACE.size, file, func, line);
    TRACE.size++;

    return true;
}

static bool trace_resize()
{
    #ifdef NTRACE
    return true;
    #endif

    if (TRACE.size != TRACE.capacity) return true;

    size_t new_capacity = 2 * TRACE.capacity;
    void  *new_data     = _log_realloc(TRACE.data, new_capacity * sizeof(source_pos));

    if (new_data == nullptr)
    {
        _stat_log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, "can't allocate memory for TRACE.data");
        abort();
        return false;
    }

    TRACE.capacity =                new_capacity;
    TRACE.data     = (source_pos *) new_data;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_pop()
{
    #ifdef NTRACE
    return;
    #endif

    assert(TRACE.size <= TRACE.capacity);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;
    TRACE.size--;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_dump(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    source_pos cur_pos = {};
    source_pos_ctor(&cur_pos, cur_file, cur_func, cur_line);
    trace_el_dump  (&cur_pos, 0);

    #ifdef NTRACE
    return;
    #endif

    for (size_t i = 1; i <= TRACE.size; ++i)
    {
        trace_el_dump(TRACE.data + (TRACE.size - i), i);
    }
}

static void trace_el_dump(const source_pos *const src_pos, const size_t index)
{
    _stat_log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d:" HTML_COLOR_CANCEL "\n", index);
    source_pos_dump(src_pos);
}

//--------------------------------------------------------------------------------------------------------------------------------
// log_print
//--------------------------------------------------------------------------------------------------------------------------------

static void log_tab()
{
    for (size_t i = 0; i < LOG_TAB; ++i) { fputc('\t', LOG_STREAM); }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_print(const char *log_buff, bool is_tab)
{
    assert(log_buff                    != nullptr);
    assert(log_buff[LOG_BUFF_SIZE - 1] ==    '\0');

    for (const char *log_pos = log_buff; *log_pos != '\0'; log_pos++)
    {
        if (*log_pos == '\n') { fputc(*log_pos, LOG_STREAM); is_tab = true; }
        else
        {
            if (is_tab) { log_tab(); is_tab = false; }
            fputc(*log_pos, LOG_STREAM);
        }
    }
}

static void log_buff_size_error(const char *const cur_file,
                                const char *const cur_func,
                                const int         cur_line)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    fprintf(LOG_STREAM, HTML_COLOR_DARK_RED "\n"
                        "ERROR: log_buff overflow. LOG_BUFF_SIZE = %lu\n"
                        "====================\n"
                        "    FILE: %s\n"
                        "FUNCTION: %s\n"
                        "    LINE: %d\n"
                        "====================\n"
                        HTML_COLOR_CANCEL, LOG_BUFF_SIZE, cur_file, cur_func, cur_line);
}

#define log_buff_size_error() log_buff_size_error(__FILE__, __PRETTY_FUNCTION__, __LINE__)

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

static void _stat_log_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);
    _log_message(fmt, ap);
}

static void _log_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    char log_buff[LOG_BUFF_SIZE] = {};
    if (vsprintf (log_buff, fmt, ap) >= (int) LOG_BUFF_SIZE)
    {
        log_buff_size_error();
        return;
    }
    log_print(log_buff, false);
    va_end(ap);
}

void _log_message(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_message(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stat_log_tab_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);
}

static void _log_tab_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    char log_buff[LOG_BUFF_SIZE] = {};
    if (vsprintf (log_buff, fmt, ap) >= (int) LOG_BUFF_SIZE)
    {
        log_buff_size_error();
        return;
    }
    log_print(log_buff, true);
    va_end(ap);
}

void _log_tab_message(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

static void _log_error(const char *fmt, va_list ap)
{
    log_tab_message(HTML_COLOR_DARK_RED "\n"
                    "ERROR:\n");
    _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    trace_dump();
    log_tab_message("===================="
                    HTML_COLOR_CANCEL "\n");
}

void _log_error(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_error(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _log_error_message(const char *fmt, va_list ap)
{
    log_message(HTML_COLOR_DARK_RED);
    _log_tab_message(fmt, ap);
    log_message(HTML_COLOR_CANCEL);
}

void _log_error_message(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line,

                        const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_error_message(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stat_log_oneline_error(const char *const cur_file,
                                    const char *const cur_func,
                                    const int         cur_line,

                                    const char *fmt, ...)
{
    va_list  ap;
    va_start(ap, fmt);
    _log_oneline_error(cur_file, cur_func, cur_line, fmt, ap);
}

static void _log_oneline_error(const char *const cur_file,
                               const char *const cur_func,
                               const int         cur_line,

                               const char *fmt, va_list ap)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    log_tab_message(HTML_COLOR_DARK_RED "\n"
                    "ERROR:\n");
    _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("===================="
                    HTML_COLOR_CANCEL "\n");
}

void _log_oneline_error(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line,

                        const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_oneline_error(cur_file, cur_func, cur_line, fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

static void _log_warning(const char *fmt, va_list ap)
{
    log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                    "WARNING:\n");
    _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    trace_dump();
    log_tab_message("===================="
                    HTML_COLOR_CANCEL "\n");
}

void _log_warning(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_warning(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _log_warning_message(const char *fmt, va_list ap)
{
    log_message(HTML_COLOR_DARK_ORANGE);
    _log_tab_message(fmt, ap);
    log_message(HTML_COLOR_CANCEL);
}

void _log_warning_message(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_warning_message(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stat_log_oneline_warning(const char *const cur_file,
                                      const char *const cur_func,
                                      const int         cur_line,

                                      const char *fmt, ...)
{
    va_list  ap;
    va_start(ap, fmt);
    _log_oneline_warning(cur_file, cur_func, cur_line, fmt, ap);
}

static void _log_oneline_warning(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const char *fmt, va_list ap)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                    "WARNING:\n");
    _log_tab_message(fmt, ap);

    log_tab_message("====================\n");
    log_param_place(cur_file, cur_func, cur_line);
    log_tab_message("===================="
                    HTML_COLOR_CANCEL "\n");
}

void _log_oneline_warning(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_oneline_warning(cur_file, cur_func, cur_line, fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

static void _log_header(const char *fmt, va_list ap)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    fprintf (LOG_STREAM, "<h2>\n");
    _log_tab_message(fmt, ap);
    fprintf (LOG_STREAM, "</h2>\n");
}

void _log_header(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,
                
                 const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);

    va_list  ap;
    va_start(ap, fmt);
    _log_header(fmt, ap);

    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _log_param_place(const char *const file,
                             const char *const func,
                             const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n", file, func, line);
}

void _log_param_place(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      const char *const param_file,
                      const char *const param_func,
                      const int         param_line)
{
    trace_push(cur_file, cur_func, cur_line);
    _log_param_place(param_file, param_func, param_line);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

static void *_log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if   (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

void *_log_calloc(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  size_t number, size_t size)
{
    trace_push(cur_file, cur_func, cur_line);
    void *ret = _log_calloc(number, size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void *_log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr  == nullptr && size == 0)        return ret;
    if      (ptr  == nullptr) { ++DYNAMIC_MEMORY; return ret; }
    else if (size ==       0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

void *_log_realloc(const char *const cur_file,
                   const char *const cur_func,
                   const int         cur_line,

                   void *ptr, size_t size)
{
    trace_push(cur_file, cur_func, cur_line);
    void *ret = _log_realloc(ptr, size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}

void _log_free(const char *const cur_file,
               const char *const cur_func,
               const int         cur_line,

               void *ptr)
{
    trace_push(cur_file, cur_func, cur_line);
    _log_free(ptr);
    trace_pop();
}
