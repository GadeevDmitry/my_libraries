/** @file */
#ifndef TRACE_STATIC_H
#define TRACE_STATIC_H

#include <stdio.h>
#include <stdlib.h>

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
*   @brief source_pos dump.
*
*   @param src_pos [in] - source_pos to dump
*/
static inline void source_pos_dump(const source_pos *const src_pos);

//--------------------------------------------------------------------------------------------------------------------------------
// trace
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
// GLOBAL
//================================================================================================================================

#define $file       (src_pos->file)
#define $func       (src_pos->func)
#define $line       (src_pos->line)

#define $stk        (trc->stack_trace)
#define $size       (trc->size)
#define $capacity   (trc->capacity)

#endif //TRACE_STATIC_H