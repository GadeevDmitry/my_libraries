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

    trace_ctor();

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void LOG_STREAM_CLOSE()
{
    assert (LOG_STREAM != nullptr);

    trace_dtor();

    fprintf(LOG_STREAM, "\n");

    if (DYNAMIC_MEMORY == 0) log_message(HTML_COLOR_LIME_GREEN "DYNAMIC_MEMORY = 0. \n" HTML_COLOR_CANCEL                );
    else                     log_message(HTML_COLOR_DARK_RED   "DYNAMIC_MEMORY = %d.\n" HTML_COLOR_CANCEL, DYNAMIC_MEMORY);

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

    log_param_place(src_pos->file, src_pos->func, src_pos->line)
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

static bool trace_ctor()
{
    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    TRACE.data = log_calloc(DEFAULT_TRACE_CAPACITY, sizeof(source_pos));
    if (TRACE.data == nullptr)
    {
        //log_error();
        return false;
    }

    TRACE.size     = 0;
    TRACE.capacity = DEFAULT_TRACE_CAPACITY;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_dtor()
{
    assert(_OPEN_CLOSE_LOG_STREAM != 0);

    log_free(TRACE.data);
    TRACE.size       = 0;
    TRACE.capacity   = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool trace_push(const char *const file,
                const char *const func,
                const int         line)
{
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
    if (TRACE.size != TRACE.capacity) return true;

    size_t new_capacity = 2 * TRACE.capacity;
    void  *new_data     = log_realloc(TRACE.data, new_capacity * sizeof(source_pos));

    if (new_data == nullptr)
    {
        //log_error();
        return false;
    }

    TRACE.capacity = new_capacity;
    TRACE.data     = new_data;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_pop()
{
    assert(TRACE.size <= TRACE.capacity);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;
    TRACE.size--;
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line)
{
    assert(cur_file != nullptr);
    assert(cur_func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    source_pos cur_pos = {};
    source_pos_ctor(&cur_pos, cur_file, cur_func, cur_line);
    trace_el_dump  (&cur_pos, 0);

    for (size_t i = 1; i <= TRACE.size; ++i)
    {
        trace_el_dump(TRACE.data + (TRACE.size - i), i);
    }
}

static void trace_el_dump(const source_pos *const src_pos, const size_t index)
{
    log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d:" HTML_COLOR_CANCEL "\n", index);
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
    assert(log_buff != nullptr);

    if (log_buff[LOG_BUFF_SIZE - 1] != '\0') { log_error("\nLOG_BUFF OVERFLOW\n"); }

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

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

static void log_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap; va_start(ap, fmt);
    log_message(fmt, ap);
}

static void log_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf (log_buff, fmt, ap);
    log_print(log_buff,   false);

    va_end(ap);
}

void log_message(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,
                 
                 const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);
    va_list  ap; va_start(ap, fmt);
    log_message(fmt, ap);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_tab_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap; va_start(ap, fmt);
    log_tab_message(fmt, ap);
}

static void log_tab_message(const char *fmt, va_list ap)
{
    assert(fmt != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf (log_buff, fmt, ap);
    log_print(log_buff,    true);

    va_end(ap);
}

void log_tab_message(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,
                     
                     const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);
    va_list ap; va_start(ap, fmt);
    log_tab_message(fmt, ap);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_header(const char *fmt, va_list ap)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) { va_end(ap); return; }

    fprintf (LOG_STREAM, "<h2>\n");
    va_list ap; va_start(ap, fmt);
    log_tab_message(fmt, ap);
    fprintf (LOG_STREAM, "</h2>\n");
}

void log_header(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,
                
                const char *fmt, ...)
{
    trace_push(cur_file, cur_func, cur_line);
    va_list ap; va_start(ap, fmt);
    log_header(fmt, ap);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_param_place(const char *const file,
                            const char *const func,
                            const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n", file, func, line);
}

void log_param_place(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     const char *const param_file,
                     const char *const param_func,
                     const int         param_line)
{
    trace_push(cur_file, cur_func, cur_line);
    log_param_place(param_file, param_func, param_line);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

static void *log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if   (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

void *log_calloc(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,
                 
                 size_t number, size_t size)
{
    trace_push(cur_file, cur_func, cur_line);
    void *ret = log_calloc(number, size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void *log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr  == nullptr && size == 0)        return ret;
    if      (ptr  == nullptr) { ++DYNAMIC_MEMORY; return ret; }
    else if (size ==       0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

void *log_realloc(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  void *ptr, size_t size)
{
    trace_push(cur_file, cur_func, cur_line);
    void *ret = log_realloc(ptr, size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}

void log_free(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              void *ptr)
{
    trace_push(cur_file, cur_func, cur_line);
    log_free(ptr);
    trace_pop();
}
