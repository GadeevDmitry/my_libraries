/** @file */
#ifndef LOG_H
#define LOG_H

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// MACRO DEFENITIONS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define HTML_COLOR_GOLD         "<font color=Gold>"
#define HTML_COLOR_DARK_RED     "<font color=DarkRed>"
#define HTML_COLOR_DARK_ORANGE  "<font color=DarkOrange>"
#define HTML_COLOR_LIME_GREEN   "<font color=LimeGreen>"
#define HTML_COLOR_MEDIUM_BLUE  "<font color=MediumBlue>"
#define HTML_COLOR_OLIVE        "<font color=Olive>"
#define HTML_COLOR_BLACK        "<font color=Black>"
#define HTML_COLOR_CANCEL       "</font>"

/**
*   @brief выводит в лог файл имя файла, имя функции, номер строки в точке вызова
*/
#define log_place()                                                             \
        log_message("\n"                                                        \
                    "    FILE: %s\n"                                            \
                    "FUNCTION: %s\n"                                            \
                    "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__)

/**
*   @brief assert с сообщением в лог файл
*/
#define log_assert(condition)                               \
            if  (!(condition))                              \
            {                                               \
			    log_message(HTML_COLOR_DARK_RED             \
                                "ASSERT FAILED: %s\n"       \
					            "         FILE: %s\n"       \
					            "     FUNCTION: %s\n"       \
					            "         LINE: %d\n"       \
                            HTML_COLOR_CANCEL        ,      \
                                                            \
				                #condition           ,      \
						        __FILE__		     ,      \
						        __PRETTY_FUNCTION__	 ,      \
						        __LINE__		      );    \
			abort();                                        \
            }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// FUNCTION DECLARATION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
*   @brief Эквивалентна fprintf(LOG_STREAM, fmt, ...)
*
*   @see log_error  (const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
void log_message       (const char *fmt, ...);

/**
*   @brief Выводит сообщение об ошибке в лог файл
*
*   @see log_message(const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
void log_error         (const char *fmt, ...);

/**
*   @brief Выводит warning в лог файл
*
*   @see log_message(const char *fmt, ...)
*   @see log_error  (const char *fmt, ...)
*/
void log_warning       (const char *fmt, ...);

/**
*   @brief HTML-заголовок
*/
void log_header        (const char *fmt, ...);

/**
*   @brief Выводит в лог файл имя файла, имя функции, номер строки
*
*   @param file [in] - имя файла
*   @param func [in] - имя функции
*   @param line [in] - номер строки
*/
void log_param_place   (const char *file,
                        const char *func,
                        const int   line);

#endif //LOG_H