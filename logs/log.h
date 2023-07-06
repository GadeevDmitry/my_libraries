/** @file */
#ifndef LOG_H
#define LOG_H

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#include <assert.h>
#include <stdlib.h>

#include "log_settings.h"
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
// TRACE SHELL
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для _trace_push(trace*).
*   Создана с целью скрыть глобальную переменную TRACE.
*
*   @see _trace_push(trace*)
*/
void log_trace_push();

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для _trace_front_upd(const char* const file, const char* const func, const int line).
*   Создана с целью скрыть глобальную переменную TRACE.
*
*   @see _trace_front_upd(const char* const file, const char* const func, const int line)
*/
void log_trace_upd(const char *const file,
                   const char *const func,
                   const int         line);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для _trace_pop(trace*).
*   Создана с целью скрыть глобальную переменную TRACE.
*
*   @see _trace_pop(trace*)
*/
void log_trace_pop();

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для _trace_dump(const trace*).
*   Создана с целью скрыть глобальную переменную TRACE.
*
*   @see _trace_dump(const trace*)
*/
void _log_trace_dump();

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение об ошибке в точке вызова. Делает TRACE dump, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file       [in] - файл в точке вызова
*   @param cur_func       [in] - функция в точке вызова
*   @param cur_line       [in] - строка в точке вызова
*
*   @param fmt            [in] - формат вывода
*/
void _log_error(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line, const char *fmt, ...);

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
                        const int         cur_line, const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_WARNING
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит warning в точке вызова. Делает TRACE dump, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file       [in] - файл в точке вызова
*   @param cur_func       [in] - функция в точке вызова
*   @param cur_line       [in] - строка в точке вызова
*
*   @param fmt            [in] - формат вывода
*/
void _log_warning(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line, const char *fmt, ...);

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
                          const int         cur_line, const char *fmt, ...);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Делает HTML-заголовок.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат заголовка
*/
void _log_header(const char *fmt, ...);

/**
*   @brief Выводит сообщение о местоположении с параметрами.
*
*   @param file [in] - файл
*   @param func [in] - функция
*   @param line [in] - номер строки
*/
void _log_param_place(const char *const file,
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
*   @see log_recalloc(void *, size_t, size_t, bool)
*   @see log_free(void *)
*/
void *_log_calloc(size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_recalloc(void *, size_t, size_t, bool)
*   @see log_free(void *)
*/
void *_log_realloc(void *ptr, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Если размер блока увеличился, инициализирует дополнительную память нулями.
*   Меняет DYNAMIC_MEMORY в зависимости от значения параметра is_nleak.
*
*   @param ptr      [in] - указатель на блок для реаллокации
*   @param old_size [in] - старый размер блока
*   @param new_size [in] - новый размер блока
*   @param is_nleak [in] - если true, то DYNAMIC_MEMORY не меняется
*
*   @return указатель на реаллоцированный блок
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*   @see log_free(void *)
*/
void *_log_recalloc(void *ptr, size_t old_size,
                               size_t new_size, const bool is_nleak);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*   @see log_recalloc(void *, size_t, size_t, bool)
*/
void _log_free(void *ptr);

//================================================================================================================================
// MACRO DEFENITIONS
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// COLORS
//--------------------------------------------------------------------------------------------------------------------------------

#define HTML_COLOR_GOLD         "<font color=Gold>"         ///< HTML директива установки цвета шрифта: Gold        (#FFD700)
#define HTML_COLOR_DARK_RED     "<font color=DarkRed>"      ///< HTML директива установки цвета шрифта: DarkRed     (#8B0000)
#define HTML_COLOR_DARK_ORANGE  "<font color=DarkOrange>"   ///< HTML директива установки цвета шрифта: DarkOrange  (#FF8C00)
#define HTML_COLOR_LIME_GREEN   "<font color=LimeGreen>"    ///< HTML директива установки цвета шрифта: LimeGreen   (#32CD32)
#define HTML_COLOR_MEDIUM_BLUE  "<font color=MediumBlue>"   ///< HTML директива установки цвета шрифта: MediumBlue  (#0000CD)
#define HTML_COLOR_OLIVE        "<font color=Olive>"        ///< HTML директива установки цвета шрифта: Olive       (#808000)
#define HTML_COLOR_BLACK        "<font color=Black>"        ///< HTML директива установки цвета шрифта: Black       (#000000)
#define HTML_COLOR_CANCEL       "</font>"                   ///< HTML директива сброса цвета шрифта к предыдущему
#define HTML_COLOR_POISON       HTML_COLOR_OLIVE            ///< <=> HTML_COLOR_OLIVE

#define BASH_COLOR_RED          "\033[1;31m"                ///< BASH директива установки цвета шрифта: light red
#define BASH_COLOR_GREEN        "\033[1;32m"                ///< BASH директива установки цвета шрифта: light green
#define BASH_COLOR_YELLOW       "\033[1;33m"                ///< BASH директива установки цвета шрифта: yellow
#define BASH_COLOR_BLUE         "\033[1;34m"                ///< BASH директива установки цвета шрифта: light blue
#define BASH_COLOR_PURPLE       "\033[1;35m"                ///< BASH директива установки цвета шрифта: light purple
#define BASH_COLOR_CYAN         "\033[1;36m"                ///< BASH директива установки цвета шрифта: light cyan
#define BASH_COLOR_WHITE        "\033[0m"                   ///< BASH директива установки цвета шрифта: white

//--------------------------------------------------------------------------------------------------------------------------------
// COMMON
//--------------------------------------------------------------------------------------------------------------------------------

#include "log_def.h"

/**
*   @brief выводит в лог имя файла, имя функции, номер строки в точке вызова.
*/
#define log_place() log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__)

/**
*   @brief выводит в лог заголовок с именем текущей функции
*/
#define log_entry() log_header(__PRETTY_FUNCTION__)

//--------------------------------------------------------------------------------------------------------------------------------
// EXCEPTIONS
//--------------------------------------------------------------------------------------------------------------------------------


#if !defined(NDEBUG) && !defined(NLOG) && !defined(LOG_NDEBUG)
/**
*   @brief assert с сообщением в лог файл
*/
#define log_assert_verbose(condition, message)              \
    if (!(condition))                                       \
    {                                                       \
        log_tab_message(HTML_COLOR_DARK_RED "\n"            \
                        "ASSERTION FAILED: %s\n",           \
                        message);                           \
        log_tab_message("====================\n");          \
        log_place();                                        \
        log_tab_message("====================\n");          \
    $   log_trace_dump();                                   \
        log_tab_message("===================="              \
                        HTML_COLOR_CANCEL "\n\n");          \
                                                            \
        fprintf(stderr, BASH_COLOR_RED                      \
                        "ASSERTION FAILED "                 \
                        BASH_COLOR_CYAN                     \
                        "(check log)\n"                     \
                        BASH_COLOR_WHITE);                  \
	    abort();                                            \
    }

#define log_assert(condition) log_assert_verbose(condition, #condition)

#else // defined(NDEBUG) || defined(NLOG) || defined(LOG_NDEBUG)
/**
*   @brief <=> assert
*/
#define log_assert_verbose(condition, message) assert(condition)
#define log_assert(        condition)          assert(condition)
#endif

#if !defined(NVERIFY) && !defined(NLOG) && !defined(LOG_NVERIFY)
/**
*   @brief "мягкий" assert с сообщением в лог
*/
#define log_verify_verbose(condition, message, ret_val)     \
    if (!(condition))                                       \
    {                                                       \
        log_tab_message(HTML_COLOR_DARK_RED "\n"            \
                        "VERIFY FAILED: %s\n",              \
                        message);                           \
        log_tab_message("====================\n");          \
        log_place();                                        \
        log_tab_message("====================\n");          \
    $   log_trace_dump();                                   \
        log_tab_message("===================="              \
                        HTML_COLOR_CANCEL "\n\n");          \
                                                            \
    $o  return ret_val;                                     \
    }

#define log_verify(condition, ret_val) log_verify_verbose(condition, #condition, ret_val)

#elif !defined(NVERIFY)
/**
*   @brief "мягкий" assert без сообщения в лог
*/
#define log_verify_verbose(condition, message, ret_val)             \
    if (!(condition)) { $o return ret_val; }
#define log_verify(condition, ret_val)                              \
    if (!(condition)) { $o return ret_val; }

#else
#define log_verify_verbose(condition, message, ret_val)
#define log_verify(condition, ret_val)
#endif

#endif //LOG_H