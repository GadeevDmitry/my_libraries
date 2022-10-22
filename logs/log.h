#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define YELLOW "<font color=Gold>"
#define RED    "<font color=DarkRed>"
#define GREEN  "<font color=LimeGreen>"
#define BLUE   "<font color=MediumBlue>"
#define POISON "<font color=Olive>"
#define BLACK  "<font color=Black>"
#define CANCEL "</font>\n"
#define USUAL  BLACK

#ifndef LOG_FILE
#define LOG_FILE "log.html"
#endif

FILE *LOG_STREAM    = nullptr;

/*___________________________FUNCTION_DECLARATION___________________________*/

static int   OPEN_LOG_STREAM  ();

static void  CLOSE_LOG_STREAM ();
static void  log_message      (const char *fmt, ...);
static void  log_error        (const char *fmt, ...);
static void _log_char_ptr     (const char *str_name, const char *str, const int64_t poison, const int len);

/*__________________________________________________________________________*/

/**
*   @brief Closes log-file. Called by using atexit().
*
*   @return 1 if closing is OK. Does abort() if an ERROR found.
*/

static void CLOSE_LOG_STREAM()
{
    assert (LOG_STREAM != nullptr);

    fprintf(LOG_STREAM, "\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

/**
*   @brief Opens log-file. Ckecks if opening is OK and in this case prints message in the log-file.
*   @brief Uses atexit() to call CLOSE_LOG_STREAM() after program end.
*
*   @return 1 if checking is OK. Does abort() if an ERROR found.
*/

static int OPEN_LOG_STREAM()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    assert(LOG_STREAM != nullptr);

    setvbuf(LOG_STREAM,   nullptr, _IONBF, 0);

    fprintf(LOG_STREAM, "<pre>\n""\"%s\" OPENING IS OK\n\n", LOG_FILE);

    atexit(CLOSE_LOG_STREAM);
    return 1;
}

static int  _OPEN_CLOSE_LOG_STREAM = OPEN_LOG_STREAM();

/**
*   @brief Prints message in LOG_FILE.
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/

static void log_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(LOG_STREAM, fmt, ap);
    //fprintf (LOG_STREAM, "</font>");
}

#define log_char_ptr(str_name, poison, len) _log_char_ptr(#str_name, str_name, poison, len)

static void _log_char_ptr(const char *str_name, const char *str, const int64_t poison, const int len)
{
    if      (str ==               nullptr)  log_message("%-*s = " BLUE   "nullptr\n", len, str_name);
    else if (str == (const char *) poison)  log_message("%-*s = " POISON "poison\n" , len, str_name);
    else                                    log_message("%-*s = " USUAL  "%s\n"     , len, str_name, str);
}

/**
*   @brief Prints error-message in LOG_FILE. Before the message prints "ERROR: ".
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/

static void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf (LOG_STREAM, RED "ERROR: ");
    vfprintf(LOG_STREAM, fmt ,ap);
}

#endif //LOG_H