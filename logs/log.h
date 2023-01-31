/** @file */
#ifndef LOG_H
#define LOG_H

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#include "log_def.h"
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
*   @brief выводит в лог имя файла, имя функции, номер строки в точке вызова
*/
#define log_place() log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__, __FILE__, __PRETTY_FUNCTION__, __LINE__)

/**
*   @brief assert с сообщением в лог файл
*/
#define log_assert(condition)                                       \
            if  (!(condition))                                      \
            {                                                       \
			    log_tab_message(HTML_COLOR_DARK_RED "\n"            \
                                "ASSERTION FAILED: %s\n"            \
				                #condition);                        \
                log_tab_message("====================\n");          \
                trace_dump();                                       \
                log_tab_message("===================="              \
                                HTML_COLOR_CANCEL "\n");            \
                                                                    \
                fprintf(stderr, "ASSERTION FAILED (check log)\n");  \
			    abort();                                            \
            }


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

#include "log_undef.h"

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет в TRACE информацию о точке вызова
*
*   @param file [in] - файл
*   @param func [in] - функция
*   @param line [in] - номер строки
*
*   @return true, если все ОК, false, если не удалось выделить память для нового source_pos
*/
bool trace_push(const char *const file,
                const char *const func,
                const int         line);

/**
*   @brief Удаляет последний source_pos из TRACE
*/
void trace_pop();

/**
*   @brief trace dump
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*/
void trace_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для static void log_message(const char *, va_list) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_message(const char*, va_list)
*/
void log_message(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,
                 
                 const char *fmt, ...);

/**
*   @brief Оболочка для static void log_tab_message(const char *, va_list) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_tab_message(const char*, va_list)
*/
void log_tab_message(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,
                     
                     const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для static void log_error(const char *, va_list) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see log_error(const char*, va_list)
*/
void log_error(const char *const cur_file,
               const char *const cur_func,
               const int         cur_line,

               const char *fmt, ...);

/**
*   @brief Оболочка для static void log_error_message(const char *, va_list) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see log_error_message(const char*, va_list)
*/
void log_error_message(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line,
                       
                       const char *fmt, ...);

/**
*   @brief Оболочка для static void log_oneline_error(const char *, const char *, const int, const char *, va_list) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_oneline_error(const char*, const char*, const int, const char*, va_list)
*/
void log_oneline_error(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line,

                       const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболчка для static void log_header(const char *, va_list) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_header(const char*, va_list)
*/
void log_header(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,
                
                const char *fmt, ...);

/**
*   @brief Оболочка для static void log_param_place(const char *, const char *, const int) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_param_place(const char*, const char*, const int)
*/
void log_param_place(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     const char *const param_file,
                     const char *const param_func,
                     const int         param_line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для static void *log_calloc(size_t, size_t) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_calloc(size_t, size_t)
*/
void *log_calloc(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,
                 
                 size_t number, size_t size);

/**
*   @brief Оболочка для static void *log_realloc(void *, size_t) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_realloc(void*, size_t)
*/
void *log_realloc(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  void *ptr, size_t size);

/**
*   @brief Оболочка для static void log_free(void *) для trace_push и trace_pop.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @see log_free(void*)
*/
void log_free(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              void *ptr);

#include "log_def.h"

#endif //LOG_H