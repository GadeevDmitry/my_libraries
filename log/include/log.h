#ifndef LOG_H
#define LOG_H

//================================================================================================================================

#include <stdlib.h>
#include <assert.h>

#include "log_settings.h"

//================================================================================================================================

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

#define BOLD_LOG_SEP   "====================\n"
#define ITALIC_LOG_SEP "--------------------\n"

//================================================================================================================================

/**
*   @brief Выводит сообщение в лог. Перед каждой строкой, кроме первой, выводит LOG_TAB табов.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*
*   @see LOG_TAB
*/
void log_message(const char *fmt, ...);

/**
*   @brief Выводит сообщение в лог. Перед каждой строкой выводит LOG_TAB табов.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат вывода
*
*   @see LOG_TAB
*/
void log_tab_message(const char *fmt, ...);

/**
*   @brief Выводит сообщение об ошибке в точке вызова. Делает дамп стека trace-а, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат вывода
*/
void log_error(const char *const cur_file, const char *const cur_func, const int cur_line,
               const char *fmt, ...);

/**
*   @brief Выводит сообщение об ошибке в точке вызова. Не делает дамп стека trace-а.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат вывода
*/
void log_oneline_error(const char *const cur_file, const char *const cur_func, const int cur_line,
                       const char *fmt, ...);

/**
*   @brief Выводит warning в точке вызова. Делает дамп стека trace-а, если не определен LOG_NTRACE.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат вывода
*/
void log_warning(const char *const cur_file, const char *const cur_func, const int cur_line,
                 const char *fmt, ...);

/**
*   @brief Выводит warning в точке вызова. Не делает дамп стека trae-а.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат вывода
*/
void log_oneline_warning(const char *const cur_file, const char *const cur_func, const int cur_line,
                         const char *fmt, ...);

/**
*   @brief Вызывается в случае падения assert-а. Выводит отчет в лог, abort-ит программу.
*
*   @param cur_file       [in] - файл в точке вызова
*   @param cur_func       [in] - функция в точке вызова
*   @param cur_line       [in] - строка в точке вызова
*
*   @param report_message [in] - сообщение assert-а
*/
void log_assertion_failed(const char *const cur_file, const char *const cur_func, const int cur_line,
                          const char *report_message) __attribute__((noreturn));

/**
*   @brief Вызывается в случае падения верификатора. Выводит отчет в лог.
*
*   @param cur_file       [in] - файл в точке вызова
*   @param cur_func       [in] - функция в точке вызова
*   @param cur_line       [in] - строка в точке вызова
*
*   @param report_message [in] - сообщение верификатора
*/
void log_verification_failed(const char *const cur_file, const char *const cur_func, const int cur_line,
                             const char *report_message);

/**
*   @brief Делает HTML-заголовок.
*   Правила задания аргументов аналогичны функции printf.
*
*   @param fmt [in] - формат заголовка
*/
void log_header(const char *fmt, ...);

/**
*   @brief Выводит сообщение о местоположении с параметрами.
*
*   @param file [in] - файл
*   @param func [in] - функция
*   @param line [in] - номер строки
*/
void log_param_place(const char *const file, const char *const func, const int line);

/**
*   @brief Запрашивает динамическую память, используя calloc(). Увеличивает DYNAMIC_MEMORY, если запрос успешен.
*
*   @see DYNAMIC_MEMORY
*   @see log_realloc(void *, size_t)
*   @see log_recalloc(void *, size_t, size_t, bool)
*   @see log_free(void *)
*/
void *log_calloc(size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_recalloc(void *, size_t, size_t, bool)
*   @see log_free(void *)
*/
void *log_realloc(void *ptr, size_t size);

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
void *log_recalloc(void *ptr, size_t old_size,
                               size_t new_size, const bool is_nleak);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст.
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*   @see log_recalloc(void *, size_t, size_t, bool)
*/
void log_free(void *ptr);

//================================================================================================================================

extern size_t LOG_TAB;

//================================================================================================================================

#ifndef NLOG

#define LOG_MESSAGE(    fmt, ...) log_message    (fmt, ##__VA_ARGS__)
#define LOG_TAB_MESSAGE(fmt, ...) log_tab_message(fmt, ##__VA_ARGS__)

#define LOG_ERROR(  fmt, ...) log_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) log_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ONELINE_ERROR(  fmt, ...) log_oneline_error  (__FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ONELINE_WARNING(fmt, ...) log_oneline_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_OK_MESSAGE(     fmt, end, ...) log_message(HTML_COLOR_LIME_GREEN  fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_ERROR_MESSAGE(  fmt, end, ...) log_message(HTML_COLOR_DARK_RED    fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_WARNING_MESSAGE(fmt, end, ...) log_message(HTML_COLOR_DARK_ORANGE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_SERVICE_MESSAGE(fmt, end, ...) log_message(HTML_COLOR_MEDIUM_BLUE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_DEFAULT_MESSAGE(fmt, end, ...) log_message(HTML_COLOR_BLACK       fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_POISON_MESSAGE( fmt, end, ...) log_message(HTML_COLOR_POISON      fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)

#define LOG_TAB_OK_MESSAGE(     fmt, end, ...) log_tab_message(HTML_COLOR_LIME_GREEN  fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_TAB_ERROR_MESSAGE(  fmt, end, ...) log_tab_message(HTML_COLOR_DARK_RED    fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_TAB_WARNING_MESSAGE(fmt, end, ...) log_tab_message(HTML_COLOR_DARK_ORANGE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_TAB_SERVICE_MESSAGE(fmt, end, ...) log_tab_message(HTML_COLOR_MEDIUM_BLUE fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_TAB_DEFAULT_MESSAGE(fmt, end, ...) log_tab_message(HTML_COLOR_BLACK       fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)
#define LOG_TAB_POISON_MESSAGE( fmt, end, ...) log_tab_message(HTML_COLOR_POISON      fmt HTML_COLOR_CANCEL end, ##__VA_ARGS__)

#define LOG_HEADER(fmt, ...) log_header(fmt, ##__VA_ARGS__)

#define LOG_PARAM_PLACE(file, func, line) log_param_place(file, func, line)

#else

#define LOG_MESSAGE(    fmt, ...)
#define LOG_TAB_MESSAGE(fmt, ...)

#define LOG_ERROR(  fmt, ...)
#define LOG_WARNING(fmt, ...)

#define LOG_ONELINE_ERROR(  fmt, ...)
#define LOG_ONELINE_WARNING(fmt, ...)

#define LOG_OK_MESSAGE(     fmt, end, ...)
#define LOG_ERROR_MESSAGE(  fmt, end, ...)
#define LOG_WARNING_MESSAGE(fmt, end, ...)
#define LOG_SERVICE_MESSAGE(fmt, end, ...)
#define LOG_DEFAULT_MESSAGE(fmt, end, ...)
#define log_POISON_MESSAGE( fmt, end, ...)

#define LOG_TAB_OK_MESSAGE(     fmt, end, ...)
#define LOG_TAB_ERROR_MESSAGE(  fmt, end, ...)
#define LOG_TAB_WARNING_MESSAGE(fmt, end, ...)
#define LOG_TAB_SERVICE_MESSAGE(fmt, end, ...)
#define LOG_TAB_DEFAULT_MESSAGE(fmt, end, ...)
#define LOG_TAB_POISON_MESSAGE( fmt, end, ...)

#define LOG_HEADER(fmt, ...)

#define LOG_PARAM_PLACE(file, func, line)

#endif

//--------------------------------------------------------------------------------------------------------------------------------

#define POISON_FIELD_DUMP( field)                        LOG_TAB_MESSAGE(HTML_COLOR_POISON      field " = " "POISON"       HTML_COLOR_CANCEL "\n")
#define OK_FIELD_DUMP(     field, specified_char, value) LOG_TAB_MESSAGE(HTML_COLOR_LIME_GREEN  field " = " specified_char HTML_COLOR_CANCEL "\n", value)
#define WARNING_FIELD_DUMP(field, specified_char, value) LOG_TAB_MESSAGE(HTML_COLOR_DARK_ORANGE field " = " specified_char HTML_COLOR_CANCEL "\n", value)
#define ERROR_FIELD_DUMP(  field, specified_char, value) LOG_TAB_MESSAGE(HTML_COLOR_DARK_RED    field " = " specified_char HTML_COLOR_CANCEL "\n", value)
#define USUAL_FIELD_DUMP(  field, specified_char, value) LOG_TAB_MESSAGE(HTML_COLOR_BLACK       field " = " specified_char HTML_COLOR_CANCEL "\n", value)

//--------------------------------------------------------------------------------------------------------------------------------

#ifndef LOG_NLEAK

#define LOG_CALLOC(number, size) log_calloc (number, size)
#define LOG_REALLOC(  ptr, size) log_realloc(   ptr, size)
#define LOG_FREE(     ptr)       log_free   (   ptr      )

#define LOG_RECALLOC(ptr, old_size, new_size) log_recalloc(ptr, old_size, new_size, false)

#else

#define LOG_CALLOC(number, size) calloc  (number, size)
#define LOG_REALLOC(  ptr, size) realloc (   ptr, size)
#define LOG_FREE(     ptr)       free    (   ptr      )

#define LOG_RECALLOC( ptr, old_size, new_size) log_recalloc(ptr, old_size, new_size, true)

#endif

//--------------------------------------------------------------------------------------------------------------------------------

#define STDERR_OK_MESSAGE(   fmt, ...) fprintf(stderr, BASH_COLOR_GREEN fmt BASH_COLOR_WHITE, ##__VA_ARGS__)
#define STDERR_ERROR_MESSAGE(fmt, ...) fprintf(stderr, BASH_COLOR_RED   fmt BASH_COLOR_WHITE, ##__VA_ARGS__)

#define LOG_PLACE() LOG_PARAM_PLACE(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#define LOG_ENTRY() LOG_HEADER     (          __PRETTY_FUNCTION__          )

//--------------------------------------------------------------------------------------------------------------------------------

#ifndef LOG_NTRACE

#include "../src/trace.h"
#define $  { trace_upd_pos(__FILE__, __PRETTY_FUNCTION__, __LINE__); }
#define $i { trace_push   (); $ }
#define $o { trace_pop    ();   }

#else

#define $
#define $i
#define $o

#endif

//--------------------------------------------------------------------------------------------------------------------------------

#ifndef LOG_NDEBUG

#define LOG_ASSERT_VERBOSE(condition, message)  \
    if (!(condition)) log_assertion_failed(__FILE__, __PRETTY_FUNCTION__, __LINE__, message);

#define LOG_ASSERT(condition) LOG_ASSERT_VERBOSE(condition, #condition)

#else

#define LOG_ASSERT_VERBOSE(condition, message) assert(condition)
#define LOG_ASSERT(        condition)          assert(condition)

#endif

#ifndef LOG_NVERIFY

#define LOG_VERIFY_VERBOSE(condition, message, ret_val)                             \
    if (!(condition))                                                               \
    {                                                                               \
        log_verification_failed(__FILE__, __PRETTY_FUNCTION__, __LINE__, message);  \
    $o  return ret_val;                                                             \
    }

#define LOG_VERIFY(condition, ret_val) LOG_VERIFY_VERBOSE(condition, #condition, ret_val)

#elif !defined(NVERIFY)

#define LOG_VERIFY_VERBOSE(condition, message, ret_val) if (!(condition)) { return ret_val; }
#define LOG_VERIFY(        condition,          ret_val) if (!(conditoon)) { return ret_val; }

#else

#define LOG_VERIFY_VERBOSE(condition, message, ret_val)
#define LOG_VERIFY(        condition,          ret_val)

#endif

#endif // LOG_H
