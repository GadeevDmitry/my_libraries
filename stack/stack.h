/** @file */
#ifndef STACK_H
#define STACK_H

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура данных стек
*/
struct stack
{
    void  *data;            ///< указатель на начало массива с элементами стека

    size_t  el_size;        ///< размер элемента стека (в байтах)
    size_t     size;        ///< размер стека (количество элементов в стеке)
    size_t capacity;        ///< емкость стека (максимальное количество элементов, которое может храниться в стеке)

    const void *el_poison;  ///< указатель на POISON-элемент стека

    void (*el_dtor)      (      void *const el);    ///< указатель на dtor элемента стека
    void (*el_dump)      (const void *const el);    ///< указатель на dump элемента стека
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool _stack_ctor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const size_t el_size, const void *const el_poison                     = nullptr,
                                                               void (     *el_dtor  )(      void *const) = nullptr,
                                                               void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

stack *_stack_new(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const size_t el_size, const void *const el_poison                     = nullptr,
                                              void (     *el_dtor  )(      void *const) = nullptr,
                                              void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void _stack_dtor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

bool _stack_push(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _stack_pop(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                stack *const stk, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

bool _stack_front(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const stack *const stk, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _stack_is_empty(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

void _stack_dump(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const void *const _stk);

#include "stack_def.h"

#endif //STACK_H