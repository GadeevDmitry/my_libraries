/** @file */
#ifndef LOG_H
#define LOG_H

//================================================================================================================================
// GLOBAL
//================================================================================================================================

extern size_t LOG_TAB;

//================================================================================================================================
// MACRO DEFENITIONS
//================================================================================================================================

#include <assert.h>
#include <stdlib.h>

#define HTML_COLOR_GOLD         "<font color=Gold>"
#define HTML_COLOR_DARK_RED     "<font color=DarkRed>"
#define HTML_COLOR_DARK_ORANGE  "<font color=DarkOrange>"
#define HTML_COLOR_LIME_GREEN   "<font color=LimeGreen>"
#define HTML_COLOR_MEDIUM_BLUE  "<font color=MediumBlue>"
#define HTML_COLOR_OLIVE        "<font color=Olive>"
#define HTML_COLOR_BLACK        "<font color=Black>"
#define HTML_COLOR_CANCEL       "</font>"

#define HTML_COLOR_POISON HTML_COLOR_OLIVE

/**
*   @brief выводит в лог файл имя файла, имя функции, номер строки в точке вызова
*/
#define log_place()                                                                 \
        log_tab_message("    FILE: %s\n"                                            \
                        "FUNCTION: %s\n"                                            \
                        "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

/**
*   @brief assert с сообщением в лог файл
*/
#define log_assert(condition)                                   \
            if  (!(condition))                                  \
            {                                                   \
			    log_tab_message(HTML_COLOR_DARK_RED             \
                                "ASSERTION FAILED: %s\n"        \
					            "            FILE: %s\n"        \
					            "        FUNCTION: %s\n"        \
					            "            LINE: %d\n"        \
                                HTML_COLOR_CANCEL    ,          \
                                                                \
				                #condition           ,          \
						        __FILE__		     ,          \
						        __PRETTY_FUNCTION__	 ,          \
						        __LINE__		      );        \
                                                                \
            fprintf(stderr, "ASSERTION FAILED (check log)\n");  \
			abort();                                            \
            }

/**
*   @brief Выводит сообение об ошике в заданной параметрами точке в лог файл
*
*   @see log_error(fmt, ...)
*   @see log_message(const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
#define log_param_error(file, func, line, fmt, ...)             \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_RED                     \
                        "\nERROR:\n");                          \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_tab_message("    FILE: %s\n"                        \
                        "FUNCTION: %s\n"                        \
                        "    LINE: %d\n"    ,                   \
                    file                    ,                   \
                    func                    ,                   \
                    line                    );                  \
        log_message(HTML_COLOR_CANCEL "\n");                    \
        }

/**
*   @brief Выводит сообщение об ошибке в точке вызова в лог файл
*
*   @see log_param_error(file, func, line, fmt, ...)
*   @see log_message(const char *fmt, ...)
*   @see log_tab_message(const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
#define log_error(fmt, ...) log_param_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__);

/**
*   @brief Выводит warning в лог файл
*
*   @see log_message(const char *fmt, ...)
*   @see log_tab_message(const char *fmt, ...)
*/
#define log_warning(fmt, ...)                                   \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_ORANGE "\nWARNING:\n"); \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_message    (HTML_COLOR_CANCEL);                     \
        }

/**
*   @brief Выводит warning c параметрами в лог
*
*   @see log_warning(fmt, ...)
*   @see log_inline_warning(fmt, ...)
*/
#define log_param_warning(file, func, line, fmt, ...)           \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_ORANGE                  \
                        "\nWARNING:\n");                        \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_tab_message("    FILE: %s\n"                        \
                        "FUNCTION: %s\n"                        \
                        "    LINE: %d\n",                       \
                    file,                                       \
                    func,                                       \
                    line);                                      \
        log_message(HTML_COLOR_CANCEL);                         \
        }

/**
*   @brief warning в точке вызова
*
*   @see log_warning(fmt, ...)
*   @see log_param_warning(file, func, line, fmt, ...)
*/
#define log_inline_warning(fmt, ...) log_param_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение в лог файл, используя vfprintf()
*   Не использует log_tab() перед выводом первой строки
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @param fmt      [in] - формат строки сообщения
*
*   @see log_tab_message(const char *fmt, ...)
*/
void log_message       (const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line, const char *fmt, ...);

/**
*   @brief Выводит сообщение в лог файл, используя vfprintf()
*   Использует log_tab() перед выводом первой строки
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @param fmt      [in] - формат строки сообщения
*
*   @see log_message(const char *fmt, ...)
*/
void log_tab_message   (const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line, const char *fmt, ...);

/**
*   @brief HTML-заголовок
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @param fmt      [in] - формат строки сообщения
*/
void log_header        (const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line, const char *fmt, ...);

/**
*   @brief Оболочка для log_param_place(const char *const, const char *const, const int) для back trace.
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @see log_param_place(const char*, const char*, const int)
*/
void log_param_place(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line, const char *const param_file,
                                                 const char *const param_func,
                                                 const int         param_line);

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief trace push
*
*   @param file [in] - имя файла
*   @param func [in] - имя функции
*   @param line [in] - номер строки
*/
void trace_push(const char *const file,
                const char *const func,
                const int         line);

/**
*   @brief trace pop
*/
void trace_pop();

/**
*   @brief trace dump
*
*   @param cur_file [in] - файл ы точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*/
void trace_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для log_calloc(size_t, size_t) для back trace
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @see log_calloc(size_t, size_t)
*/
void *log_calloc(size_t number, size_t size, const char *const cur_file,
                                             const char *const cur_func,
                                             const int         cur_line);

/**
*   @brief Оболочка для log_realloc(void *, size_t) для back trace
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @see log_realloc(void*, size_t)
*/
void *log_realloc(void *ptr, size_t size, const char *const cur_file,
                                          const char *const cur_func,
                                          const int         cur_line);

/**
*   @brief Оболочка для log_free(void *) для back trace
*
*   @param cur_file [in] - файл в точке вызова (для back trace)
*   @param cur_func [in] - функция в точке вызова (для back trace)
*   @param cur_line [in] - строка в точке вызова (для back trace)
*
*   @see log_free(void*)
*/
void log_free(void *ptr, const char *const cur_file,
                         const char *const cur_func,
                         const int         cur_line);

#include "log_def.h"

#endif //LOG_H