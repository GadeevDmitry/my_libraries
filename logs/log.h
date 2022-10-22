#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>

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

/*___________________________FUNCTION_DECLARATION___________________________*/

void log_message      (const char *fmt, ...);
void log_error        (const char *fmt, ...);
void log_char_ptr     (const char *str_name, const char         *str, const char   *poison, const uint8_t len);
void log_int64_t      (const char *num_name, const int64_t num_value, const int64_t poison, const uint8_t len);

/*__________________________________________________________________________*/

#endif //LOG_H