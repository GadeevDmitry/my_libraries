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

    setvbuf(LOG_STREAM, nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n"
                        "\"%s\" OPENING IS OK\n\n", LOG_FILE);

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

    if (DYNAMIC_MEMORY == 0) log_message(HTML_COLOR_LIME_GREEN "DYNAMIC_MEMORY = 0. \n" HTML_COLOR_CANCEL);
    else                     log_message(HTML_COLOR_DARK_RED   "DYNAMIC_MEMORY = %d.\n" HTML_COLOR_CANCEL, DYNAMIC_MEMORY);

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

//--------------------------------------------------------------------------------------------------------------------------------
// source_pos
//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_ctor(source_pos *const src_pos, const char *const file,
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

    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    TRACE.data = (source_pos *) calloc(DEFAULT_TRACE_CAPACITY, sizeof(source_pos));
    if (TRACE.data == nullptr)
    {
        _log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, "can't allocate memory for TRACE.data");
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

    free(TRACE.data);

    TRACE.size     = 0;
    TRACE.capacity = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_push(const char *const file,
                 const char *const func,
                 const int         line)
{
    #ifdef NTRACE
    return true;
    #endif

    assert(file != nullptr);
    assert(func != nullptr);

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

    assert(TRACE.size <= TRACE.capacity);

    if (TRACE.size != TRACE.capacity) return true;

    size_t new_capacity = 2 * TRACE.capacity;
    void *new_data = realloc(TRACE.data, new_capacity * sizeof(source_pos));

    if (new_data == nullptr)
    {
        _log_oneline_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, "can't allocate memory for TRACE.data");
        abort();
        return false;
    }

    TRACE.capacity =            new_capacity;
    TRACE.data     = (source_pos *) new_data;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_pop()
{
    #ifdef NTRACE
    return;
    #endif

    assert(TRACE.size <= TRACE.capacity);
    assert(TRACE.size != 0);

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

static inline void trace_el_dump(const source_pos *const src_pos, const size_t index)
{
    _log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d:" HTML_COLOR_CANCEL "\n", index);
    source_pos_dump(src_pos);
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
    assert(log_buff != nullptr);
    assert(log_buff[LOG_BUFF_SIZE - 1] == '\0');

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
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    log_message(fmt, ap);
}

static inline void log_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);
    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf(log_buff, fmt, ap);

    log_print(log_buff, false);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_tab_message(const char *fmt, ...)
{
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    log_tab_message(fmt, ap);
}

static inline void log_tab_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);
    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf(log_buff, fmt, ap);

    log_print(log_buff, true);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

void _log_error(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_tab_message(HTML_COLOR_DARK_RED "\n"
                                         "ERROR:\n");
    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_tab_message("====================\n");
    _trace_dump();
    _log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_error_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_message(HTML_COLOR_DARK_RED);

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_message(HTML_COLOR_CANCEL);
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_oneline_error(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line, const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_tab_message(HTML_COLOR_DARK_RED "\n"
                                         "ERROR:\n");

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_tab_message("====================\n");
    _log_param_place(cur_file, cur_func, cur_line);
    _log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

void _log_warning(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                                            "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_tab_message("====================\n");
    _trace_dump();
    _log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_warning_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_message(HTML_COLOR_DARK_ORANGE);

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_message(HTML_COLOR_CANCEL);
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_oneline_warning(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line, const char *fmt, ...)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    _log_tab_message(HTML_COLOR_DARK_ORANGE "\n"
                                            "WARNING:\n");

    va_list ap;
    va_start(ap, fmt);
    _log_tab_message(fmt, ap);

    _log_tab_message("====================\n");
    _log_param_place(cur_file, cur_func, cur_line);
    _log_tab_message("====================" HTML_COLOR_CANCEL "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

void _log_header(const char *fmt, ...)
{
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
    assert(file != nullptr);
    assert(func != nullptr);

    _log_tab_message("    FILE: %s\n"
                     "FUNCTION: %s\n"
                     "    LINE: %d\n", file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

void *_log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0)
        return nullptr;

    void *ret = calloc(number, size);
    if (ret == nullptr)
        return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *_log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if (ptr == nullptr && size == 0)
        return ret;
    if (ptr == nullptr)
    {
        ++DYNAMIC_MEMORY;
        return ret;
    }
    else if (size == 0)
    {
        --DYNAMIC_MEMORY;
        return ret;
    }

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _log_free(void *ptr)
{
    if (ptr == nullptr)
        return;

    --DYNAMIC_MEMORY;
    free(ptr);
}
