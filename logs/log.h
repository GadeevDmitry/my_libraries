/** @file */
#ifndef LOG_H
#define LOG_H

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#include <assert.h>
#include <stdlib.h>

extern size_t LOG_TAB;

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение в лог. Перед каждой строкой, кроме первой, выводит LOG_TAB табов.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*
*   @see LOG_TAB
*/
void _log_message(const char *fmt, ...);

/**
*   @brief Выводит сообщение в лог. Перед каждой строкой выводит LOG_TAB табов.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*
*   @see LOG_TAB
*/
void _log_tab_message(const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение об ошибке. Делает TRACE dump, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*/
void _log_error(const char *fmt, ...);

/**
*   @brief Выводит сообщение HTML_COLOR_DARK_RED цветом.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*/
void _log_error_message(const char *fmt, ...);

/**
*   @brief Выводит сообщение об ошибке с координатами в точке вызова.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат вывода
*/
void _log_oneline_error(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line,

                        const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит warning. Делает TRACE dump, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*/
void _log_warning(const char *fmt, ...);

/**
*   @brief Выводит сообщение HTML_COLOR_DARK_ORANGE цветом.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*/
void _log_warning_message(const char *fmt, ...);

/**
*   @brief Выводит warning с координатами в точке вызова.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат warning-а
*/
void _log_oneline_warning(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Делает HTML-заголовок.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат заголовка
*/
static void _log_header(const char *fmt, ...);

/**
*   @brief Выводит сообщение о местоположении с параметрами.
*
*   @param file [in] - файл
*   @param func [in] - функция
*   @param line [in] - номер строки
*/
static void _log_param_place(const char *const file,
                             const char *const func,
                             const int         line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Запрашивает динамическую память, используя calloc(). Увеличивает DYNAMIC_MEMORY, если запрос успешен.
*
*   @see DYNAMIC_MEMORY
*   @see log_realloc(void *, size_t)
*   @see log_free(void *)
*/
static void *_log_calloc(size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_free(void *)
*/
static void *_log_realloc(void *ptr, size_t size);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*/
static void _log_free(void *ptr);

//================================================================================================================================
// MACRO DEFENITIONS
//================================================================================================================================

#define HTML_COLOR_GOLD         "<font color=Gold>"
#define HTML_COLOR_DARK_RED     "<font color=DarkRed>"
#define HTML_COLOR_DARK_ORANGE  "<font color=DarkOrange>"
#define HTML_COLOR_LIME_GREEN   "<font color=LimeGreen>"
#define HTML_COLOR_MEDIUM_BLUE  "<font color=MediumBlue>"
#define HTML_COLOR_OLIVE        "<font color=Olive>"
#define HTML_COLOR_BLACK        "<font color=Black>"
#define HTML_COLOR_CANCEL       "</font>"
#define HTML_COLOR_POISON       HTML_COLOR_OLIVE

#include "log_def.h"

/**
*   @brief выводит в лог имя файла, имя функции, номер строки в точке вызова.
*/
#define log_place() log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__)

/**
*   @brief assert с сообщением в лог файл
*/
#ifndef LOG_NDEBUG
#define log_assert(condition)                                       \
            if  (!(condition))                                      \
            {                                                       \
			    log_tab_message(HTML_COLOR_DARK_RED "\n"            \
                                "ASSERTION FAILED: %s\n",           \
				                #condition);                        \
                log_tab_message("====================\n");          \
                trace_dump();                                       \
                log_tab_message("===================="              \
                                HTML_COLOR_CANCEL "\n\n");          \
                                                                    \
                fprintf(stderr, "ASSERTION FAILED (check log)\n");  \
			    abort();                                            \
            }
#else //defined LOG_NDEBUG
#define log_assert(condition) ;
#endif

/**
*   @brief "мягкий" assert с сообщением в лог
*/
#ifndef LOG_NVERIFY
#define log_verify(condition, ret_val)                              \
        if (!(condition))                                           \
        {                                                           \
            log_tab_message(HTML_COLOR_DARK_RED "\n"                \
                            "VERIFY FAILED: %s\n",                  \
                            #condition);                            \
            log_tab_message("====================\n");              \
            trace_dump();                                           \
            log_tab_message("===================="                  \
                            HTML_COLOR_CANCEL "\n\n");              \
                                                                    \
            return ret_val;                                         \
        }
#else //defined LOG_NVERIFY
#define log_verify(condition, ret_val) ;
#endif

#endif //LOG_H