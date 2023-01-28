/** @file */
#ifndef LOG_STATIC_H
#define LOG_STATIC_H

#include "../stack/stack.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

struct source_pos
{
    const char *file;
    const char *func;
    int         line;
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

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

#endif //LOG_STATIC_H