/** @file */
#ifndef LOG_STATIC_H
#define LOG_STATIC_H

#include "log.h"
#include "log_undef.h"

#include "../stack/stack.h"
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
static int  LOG_STREAM_OPEN  ();

/**
*   @brief Закрывает лог файл
*
*   @note Вызвана с помощью atexit() в LOG_STREAM_OPEN()
*
*   @see LOG_STREAM_OPEN()
*/
static void LOG_STREAM_CLOSE ();

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
*   @brief Выводит в лог файл имя файла, имя функции, номер строки
*
*   @param file [in] - имя файла
*   @param func [in] - имя функции
*   @param line [in] - номер строки
*/
static void log_param_place    (const char *file,
                                const char *func,
                                const int   line);

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
*   @brief source_pos dtor
*/
static void source_pos_dtor(void *const _src_pos);

/**
*   @brief source_pos dump
*/
static void source_pos_dump(const void *const _src_pos);

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief trace ctor
*/
static void trace_ctor();

/**
*   @brief trace dtor
*/
static void trace_dtor();

//--------------------------------------------------------------------------------------------------------------------------------
// TRACE::dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп элемента TRACE
*
*   @param src_pos [in] - элемент TRACE
*   @param index   [in] - номер элемента
*/
static void trace_el_dump(const source_pos *const src_pos, const size_t index);

/**
*   @brief Возвращает указатель на элемент TRACE с номером index
*/
static void *trace_get(const size_t index);

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
static void *log_calloc  (size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_free(void *)
*/
static void *log_realloc (void *ptr, size_t size);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*/
static void  log_free    (void *ptr);

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#define LOG_FILE "log.html"                              ///< имя лог файла

static FILE  *LOG_STREAM            = nullptr;           ///< лог файл
static int   _OPEN_CLOSE_LOG_STREAM = LOG_STREAM_OPEN(); ///< равна 0, если не удалось открыть лог файл, равна 1 иначе
static int    DYNAMIC_MEMORY        = 0;                 ///< счётчик указателей на динамическую память
static size_t LOG_BUFF_SIZE         = 100000;            ///< максимальное количество символов, которое можно вывести за один запрос log_message()
static stack  TRACE                 = {};                ///< стек для back trace

/**
*   @brief POISON-значения source_pos
*/
source_pos SOURCE_POS_POISON        =
{
    (const char *) 0xABADBABE   ,   // file
    (const char *) 0xDEADBEEF   ,   // func
    (int)          0xFEEDFACE   ,   // line
};

#include "log_def.h"

#endif //LOG_STATIC_H