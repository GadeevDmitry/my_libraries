/** @file */
#ifndef TRACE_STATIC_H
#define TRACE_STATIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "trace_settings.h"

#define LOG_NTRACE
#include "../log.h"


//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// source_pos
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор source_pos.
*/
static bool source_pos_verify(const source_pos *const src_pos);

/**
*   @brief Dump source_pos, если структура невалидна.
*   Валидность определяется верификатором source_pos_verify(const source_pos* src_pos).
*
*   @see source_pos_verify(const source_pos* src_pos)
*/
static void source_pos_debug_dump(const source_pos *const src_pos);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief source_pos ctor.
*
*   @param src_pos [out] -  source_pos to ctor
*
*   @param file    [in]  - имя файла
*   @param func    [in]  - имя функции
*   @param line    [in]  - номер строки
*/
static inline void source_pos_ctor(source_pos *const src_pos, const char *const file,
                                                              const char *const func,
                                                              const int         line);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief source_pos ctor.
*
*   @param src_pos [out] - source_pos to ctor
*   @param sample  [in]  - source_pos with fields for assignment
*/
static inline void source_pos_ctor(source_pos *const src_pos, const source_pos *const sample);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief source_pos dump.
*
*   @param src_pos [in] - source_pos to dump
*/
static inline void source_pos_dump(const source_pos *const src_pos);

//--------------------------------------------------------------------------------------------------------------------------------
// trace
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор trace.
*/
static bool trace_verify(const trace *const trc);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Dump trace, если структура невалидна.
*   Валидность определяется верификаторм trace_verify(const trace* trc).
*
*   @see trace_verify(const trace* trc)
*/
static void trace_debug_dump(const trace *const trc);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Debug dump trace.stack_trace поля.
*/
static void trace_stack_debug_dump(const trace *const trc);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Реалоцирует память в стеке вызовов.
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool trace_resize(trace *const trc);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп элемента стека вызовов.
*
*   @param src_pos [in] - элемент стека вызовов
*   @param index   [in] - индекс элемента в стеке
*/
static inline void trace_el_dump(const source_pos *const src_pos, const size_t index);

//================================================================================================================================
// MACRO
//================================================================================================================================

#define $file       (src_pos->file)
#define $func       (src_pos->func)
#define $line       (src_pos->line)

#define $front      (trc->front_trace)
#define $stk        (trc->stack_trace)
#define $size       (trc->size)
#define $capacity   (trc->capacity)

#if   !defined(NVERIFY) && !defined(NLOG) && !defined(LOG_NVERIFY)

/**
*   @brief Оболочка для верификатора trace с выводом в лог.
*/
#define log_trace_verify(trc, ret_val)                              \
    if (!trace_verify(trc))                                         \
    {                                                               \
        log_tab_message(HTML_COLOR_DARK_RED "\n"                    \
                        "ERROR: "                                   \
                        "TRACE VERIFY FAILED \n"                    \
                        "====================\n");                  \
        trace_debug_dump(trc);                                      \
        log_tab_message("===================="                      \
                        HTML_COLOR_CANCEL "\n\n");                  \
                                                                    \
        return ret_val;                                             \
    }

/**
*   @brief Оболочка для верификатора source_pos с выводом в лог.
*/
#define log_src_pos_verify(trc, src_pos, ret_val)                   \
    if (!source_pos_verify(src_pos))                                \
    {                                                               \
        log_tab_error_message("\n" "ERROR: "                        \
                              "SOURCE_POS VERIFY FAILED\n"          \
                              "====================", "\n");        \
        source_pos_debug_dump(src_pos);                             \
                                                                    \
        log_tab_message(HTML_COLOR_DARK_RED                         \
                        "====================\n");                  \
        _trace_dump(trc);                                           \
        log_tab_message("===================="                      \
                        HTML_COLOR_CANCEL "\n\n");                  \
                                                                    \
        return ret_val;                                             \
    }

#elif !defined(NVERIFY)

/**
*   @brief Оболочка для верификатора trace без вывода в лог
*/
#define log_trace_verify(trc, ret_val)                              \
    if (!trace_verify(trc)) { return ret_val; }

/**
*   @brief Оболочка для верификатора source_pos без вывода в лог
*/
#define log_src_pos_verify(trc, src_pos, ret_val)                   \
    if (!source_pos_verify(src_pos)) { return ret_val; }

#else
#define log_trace_verify(trc, ret_val)
#define log_src_pos_verify(trc, src_pos, ret_val)
#endif

#endif //TRACE_STATIC_H