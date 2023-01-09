#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "log_static.h"
#include "log.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// GLOBAL
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

size_t LOG_TAB = 0;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// STATIC FUNCTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

static void LOG_STREAM_CLOSE()
{
    assert (LOG_STREAM != nullptr);

    fprintf(LOG_STREAM, "\n");

    if (DYNAMIC_MEMORY == 0) log_message(HTML_COLOR_LIME_GREEN "DYNAMIC_MEMORY = 0. \n" HTML_COLOR_CANCEL                );
    else                     log_message(HTML_COLOR_DARK_RED   "DYNAMIC_MEMORY = %d.\n" HTML_COLOR_CANCEL, DYNAMIC_MEMORY);

    fprintf(LOG_STREAM, "\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

static void log_tab()
{
    for (size_t i = 0; i < LOG_TAB; ++i) log_message("\t");
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// USER FUNCTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//================================================================================================================================
// LOG_OUTPUT
//================================================================================================================================

void log_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);

    log_tab ();
    vfprintf(LOG_STREAM, fmt, ap);

    va_end(ap);
}

void log_warning(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    log_tab ();
    fprintf (LOG_STREAM, HTML_COLOR_DARK_ORANGE "WARNING: ");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, HTML_COLOR_CANCEL);

    va_end(ap);
}

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

void log_param_place(const char   *file,
                     const char   *func,
                     const int     line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab();
    log_message("\n"
                "    FILE: %s\n"
                "FUNCTION: %s\n"
                "    LINE: %d\n", file, func, line);
}

//================================================================================================================================
// LOG_MEMORY
//================================================================================================================================

void *log_calloc(size_t number, size_t size)
{
    if ((number * size) == 0) return nullptr;

    void *ret = calloc(number, size);
    if   (ret == nullptr) return nullptr;

    ++DYNAMIC_MEMORY;
    return ret;
}

void *log_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if      (ptr  == nullptr && size == 0)        return ret;
    if      (ptr  == nullptr) { ++DYNAMIC_MEMORY; return ret; }
    else if (size ==       0) { --DYNAMIC_MEMORY; return ret; }

    return ret;
}

void log_free(void *ptr)
{
    if (ptr == nullptr) return;

    --DYNAMIC_MEMORY;
    free(ptr);
}