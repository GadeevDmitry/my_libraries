#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "log_static.h"
#include "log.h"

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

    fprintf(LOG_STREAM, "\n\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// USER FUNCTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void log_message(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list  ap;
    va_start(ap, fmt);

    vfprintf(LOG_STREAM, fmt, ap);

    va_end(ap);
}

void log_error(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

    fprintf (LOG_STREAM, HTML_COLOR_DARK_RED "ERROR: ");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, HTML_COLOR_CANCEL);

    va_end(ap);
}

void log_warning(const char *fmt, ...)
{
    if (_OPEN_CLOSE_LOG_STREAM == 0) return;

    va_list ap;
    va_start(ap, fmt);

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

    log_message("\n"
                "    FILE: %s\n"
                "FUNCTION: %s\n"
                "    LINE: %d\n", file, func, line);
}