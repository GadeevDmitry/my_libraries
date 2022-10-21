#ifndef LOG_H
#define LOG_H

#include <stdasrg.h>

#define YELLOW "<font color=Gold"
#define RED    "<font color=DarkRed"
#define GREEN  "<font color=LimeGreen"
#define BLUE   "<font color=MediumBlue"
#define POISON "<font color=Olive"
#define BLACK  "<font color=Black"
#define USUAL  BLACK

#ifndef LOG_FILE
#define LOG_FILE "log.html"
#endif

FILE *LOG_STREAM    = nullptr;

/**
*   @brief Closes log-file. Called by using atexit().
*
*   @return 1 if closing is OK. Does abort() if an ERROR found.
*/

void CLOSE_LOG_STREAM()
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

int OPEN_LOG_STREAM()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    assert(LOG_STREAM != nullptr);

    setvbuf(LOG_STREAM,   nullptr, _IONBF, 0);

    fprintf(LOG_STREAM, "<pre>\n""\"%s\" OPENING IS OK\n\n", LOG_FILE);

    atexit(CLOSE_LOG_STREAM);
    return 1;
}

int  _OPEN_CLOSE_LOG_STREAM = OPEN_LOG_STREAM();

/**
*   @brief Prints message in LOG_FILE.
*
*   @param col [in] - color to print the message
*/

void log_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(LOG_STREAM, fmt, ap);
    //fprintf (LOG_STREAM, "</font>");
}

#endif //LOG_H