/** @file */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "log_static.h"
#include "log.h"

//================================================================================================================================
// GLOBAL
//================================================================================================================================

size_t LOG_TAB = 0; ///< количество табов, необходимое отступить перед записью в лог

//================================================================================================================================
// STATIC FUNCTION
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

static void source_pos_dtor(void *const _src_pos)
{
    source_pos *const src_pos = (source_pos *) _src_pos;
    *src_pos = SOURCE_POS_POISON;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void source_pos_dump(const void *const _src_pos)
{
    const source_pos *const src_pos = (const source_pos *) _src_pos;

    log_tab_message("source_pos (address: %p)\n"
                    "{\n",           src_pos);
    LOG_TAB++;

    if (src_pos == nullptr) { LOG_TAB--; log_tab_message("}\n"); return; }

    if      (src_pos->file == SOURCE_POS_POISON.file) poison_field_dump("file");
    else if (src_pos->file == nullptr)                error_field_dump ("file", "%p", src_pos->file);
    else                                              usual_field_dump ("file", "%p", src_pos->file);

    if      (src_pos->func == SOURCE_POS_POISON.func) poison_field_dump("func");
    else if (src_pos->func == nullptr)                error_field_dump ("func", "%s", src_pos->func);
    else                                              usual_field_dump ("func", "%s", src_pos->func);

    if      (src_pos->line == SOURCE_POS_POISON.line) poison_field_dump("line");
    else                                              usual_field_dump ("line", "%d", src_pos->line);

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

static void trace_ctor()
{
    stack_ctor(&TRACE, sizeof(source_pos), &SOURCE_POS_POISON, source_pos_dtor, source_pos_dump);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_dtor()
{
    stack_dtor(&TRACE);
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_push(const char *const file,
                const char *const func,
                const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    source_pos cur_pos = {};
    source_pos_ctor(&cur_pos, file, func, line);

    stack_push(&TRACE, &cur_pos);
    source_pos_dtor   (&cur_pos);
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_pop()
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    stack_pop(&TRACE);
}

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE::dump
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
        trace_el_dump((const source_pos *) trace_get(TRACE.size - i), i);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_el_dump(const source_pos *const src_pos, const size_t index)
{
    assert(src_pos != nullptr);

    log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d:" HTML_COLOR_CANCEL "\n", index);
    log_tab_message("    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n",

                    src_pos->file   ,
                    src_pos->func   ,
                    src_pos->line);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void *trace_get(const size_t index)
{
    assert(index < TRACE.capacity);

    return (char *) TRACE.data + TRACE.el_size * index;
}

//===============================================================================================================================
// USER FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

void log_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf (log_buff, fmt, ap);
    log_print(log_buff, false);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_tab_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    char log_buff[LOG_BUFF_SIZE] = {};
    vsprintf (log_buff, fmt, ap);
    log_print(log_buff, true);

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_header(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    log_tab ();
    fprintf (LOG_STREAM, "<h2>\n");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, "</h2>\n");

    va_end(ap);
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_param_place(const char   *file,
                     const char   *func,
                     const int     line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_message("\n"
                "    FILE: %s\n"
                "FUNCTION: %s\n"
                "    LINE: %d\n", file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

void *log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if   (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr  == nullptr && size == 0)        return ret;
    if      (ptr  == nullptr) { ++DYNAMIC_MEMORY; return ret; }
    else if (size ==       0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}
