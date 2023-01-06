/** @file */
#ifndef STACK_H
#define STACK_H

#include "stack_settings.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура данных стек
*/
struct stack
{
    void  *data;        ///< указатель на начало массива с элементами стека

    size_t  el_size;    ///< размер элемента стека (в байтах)
    size_t     size;    ///< размер стека (количество элементов в стеке)
    size_t capacity;    ///< емкость стека (максимальное количество элементов, которое может храниться в стеке)
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack ctor dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack ctor
*
*   @param stk     [in] - стек
*   @param el_size [in] - размер элемента стека (в байтах)
*
*   @return true, если всё ОК, false в случае ошибки
*
*   @see void* stack_new(const size_t el_size)
*   @see void stack_dtor(stack *const stk)
*/
bool stack_ctor(stack *const stk, const size_t el_size);

/**
*   @brief Создаёт стек в динамической памяти
*
*   @param el_size [in] - размер элемента стека (в байтах)
*
*   @return указаталь на созданный стек, nullptr в случае ошибки
*
*   @see void stack_ctor(stack *const stk, const size_t el_size)
*   @see void stack_dtor(stack *const stk)
*/
void *stack_new(const size_t el_size);

/**
*   @brief Stack dtor
*
*   @see void stack_ctor(stack *const stk, const size_t el_size)
*   @see void* stack_new(const size_t el_size)
*/
void stack_dtor(stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack main
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack push
*
*   @see bool stack_pop(stack *const stk)
*/
bool stack_push(stack *const stk, const void *const new_el);

/**
*   @brief Stack pop
*
*   @see bool stack_push(stack *const stk, const void *const new_el)
*/
bool stack_pop(stack *const stk);

/**
*   @brief Возвращает указатель на вершину стека stk
*/
void *stack_front(stack *const stk);

/**
*   @brief Возвращает true, если стек stk пустой, false иначе
*/
bool stack_empty(stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack user dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack dump
*/
void stack_dump(const stack *const stk, const char *file, const char *func, const int line);

#define stack_dump(stk) stack_dump(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#endif //STACK_H