/** @file */
#ifndef LOG_STATIC_H
#define LOG_STATIC_H

#include "log.h"
#include "log_undef.h"

#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура данных source_pos для определения позиции в исходных файлах.
*/
struct source_pos
{
    const char *file;   ///< имя файла
    const char *func;   ///< имя функции
    int         line;   ///< номер строки
};

/**
*   @brief Стек cтруктур source_pos.
*/
struct trace
{
    source_pos *data;   ///< массив source_pos
    size_t      size;   ///< размер .data
    size_t  capacity;   ///< емкость .data
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// OPEN-CLOSE
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Открывает и настраивает лог файл
*
*   @return 0, если не удалось открыть лог файл, 1 иначе
*/
static int  LOG_STREAM_OPEN();

/**
*   @brief Закрывает лог файл
*
*   @note Вызвана с помощью atexit() в LOG_STREAM_OPEN()
*
*   @see LOG_STREAM_OPEN()
*/
static void LOG_STREAM_CLOSE();

//--------------------------------------------------------------------------------------------------------------------------------
// source_pos
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief source_pos ctor
*/
static void source_pos_ctor(source_pos *const src_pos,  const char *const file,
                                                        const char *const func,
                                                        const int         line);

/**
*   @brief source_pos dump
*/
static void source_pos_dump(const source_pos *const src_pos);

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief tarce ctor
*
*   @return true, если все ОК, false, если не удалось выделить память для TRACE.data
*/
static bool trace_ctor();

/**
*   @brief trace dtor
*/
static void trace_dtor();

/**
*   @brief Увеличивает память для TRACE.data в два раза, если TRACE.size = TRACE.capacity
*
*   @return true, если все ОК, false, если не удалось выл=делить память для TRACE.data
*/
static bool trace_resize();

/**
*   @brief Дамп элемента TRACE.
*/
static void trace_el_dump(const source_pos *const src_pos, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------
// log_print
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит LOG_TAB табов в лог файл
*
*   @see LOG_TAB
*/
static void log_tab();

/**
*   @brief Выводит содержимое буфера в лог, вставляя табы перед каждой новой строкой
*
*   @param log_buff [in] - буфер для вывода
*   @param is_tab   [in] - равна true, если перед первой строкой необходимы табы
*/
static void log_print(const char *log_buff, bool is_tab);

/**
*   @brief Выводит сообщения о переполнении log_buff, который используется для вывода сообщений в лог.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*/
static void log_buff_size_error(const char *const cur_file,
                                const char *const cur_func,
                                const int         cur_line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение в лог, используя vsprintf.
*   Не выводит табы перед первой строкой.
*
*   @param fmt [in] - формат вывода
*/
static void log_message(const char *fmt, ...);

/**
*   @brief Выводит сообщение в лог, используя vsprintf.
*   Не выводит табы перед первой строкой.
*
*   @param fmt [in] - формат вывода
*   @param ap  [in] - переменный список аргументов
*/
static void log_message(const char *fmt, va_list ap);

/**
*   @brief Выоводит сообщение в лог, используя vsprintf.
*   Выводит табы перед первой строкой.
*
*   @param fmt [in] - формат вывода
*/
static void log_tab_message(const char *fmt, ...);

/**
*   @brief Выоводит сообщение в лог, используя vsprintf.
*   Выводит табы перед первой строкой.
*
*   @param fmt [in] - формат вывода
*   @param ap  [in] - переменный список аргументов
*/
static void log_tab_message(const char *fmt, va_list ap);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_ERROR
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение об ошибке и полный back trace.
*
*   @param fmt [in] - формат сообщения об ошибке
*   @param ap  [in] - переменный список аргументов
*/
static void log_error(const char *fmt, va_list ap);

/**
*   @brief Выводит сообщение красным цветом
*
*   @param fmt [in] - формат вывода
*   @param ap  [in] - переменный список аргументов
*/
static void log_error_message(const char *fmt, va_list ap);

/**
*   @brief Выводит сообщение об ошибке в точке вызова (без back trace)
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат сообщения об ошибке
*/
static void log_oneline_error(const char *const cur_file,
                              const char *const cur_func,
                              const char *const cur_line,

                              const char *fmt, ...);

/**
*   @brief Выводит сообщение об ошибке в точке вызова (без back trace)
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param fmt      [in] - формат сообщения об ошибке
*   @param ap       [in] - переменный список аргументов
*/
static void log_oneline_error(const char *const cur_file,
                              const char *const cur_func,
                              const int         cur_line,

                              const char *fmt, va_list ap);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_SMTH
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Делает HTML-заголовок, используя vsprintf.
*
*   @param fmt [in] - формат заголовка
*   @param ap  [in] - переменный список аргументов
*/
static void log_header(const char *fmt, va_list ap);

/**
*   @brief Выводит сообщение о местоположении с параметрами.
*
*   @param file [in] - файл
*   @param func [in] - функция
*   @param line [in] - номер строки
*/
static void log_param_place(const char *const file,
                            const char *const func,
                            const int         line);

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Запрашивает динамическую память, используя calloc(). Увеличивает DYNAMIC_MEMORY, если запрос успешен
*
*   @see DYNAMIC_MEMORY
*   @see log_realloc(void *, size_t)
*   @see log_free(void *)
*/
static void *log_calloc(size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_free(void *)
*/
static void *log_realloc(void *ptr, size_t size);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*/
static void log_free(void *ptr);

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#define LOG_FILE "log.html"                              ///< имя лог файла
static FILE  *LOG_STREAM            = nullptr;           ///< лог файл
static int   _OPEN_CLOSE_LOG_STREAM = LOG_STREAM_OPEN(); ///< равна 0, если не удалось открыть лог файл, равна 1 иначе

static int    DYNAMIC_MEMORY        = 0;                 ///< счётчик указателей на динамическую память
static size_t LOG_BUFF_SIZE         = 100000;            ///< максимальное количество символов, которое можно вывести за один запрос log_message()

static trace        TRACE                  = {};         ///< для back trace
static const size_t DEFAULT_TRACE_CAPACITY =  4;         ///< начальная емкость TRACE

#endif //LOG_STATIC_H