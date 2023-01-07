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
// stack ctor dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack ctor
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison         = nullptr,
                                                        void (*el_dtor) (      void *const) = nullptr,
                                                        void (*el_dump) (const void *const) = nullptr);

/**
*   @brief Создаёт стек в динамической памяти
*
*   @return указаталь на созданный стек, nullptr в случае ошибки
*/
void *stack_new(const size_t el_size,   const void *const el_poison         = nullptr,
                                        void (*el_dtor) (      void *const) = nullptr,
                                        void (*el_dump) (const void *const) = nullptr);

/**
*   @brief Stack dtor
*/
void stack_dtor(void *const stk);

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
*
*   @see stack_inline_dump(stk)
*/
void stack_dump(const void *const _stk);

/**
*   @brief Stack dump с сообщением о файле, функции, строке в точке вызова
*
*   @see void stack_dump(const void *const _stk)
*/
#define stack_inline_dump(stk)                                                      \
        log_message("stack dump called in\n"                                        \
                    "    FILE: %s\n"                                                \
                    "FUNCTION: %s\n"                                                \
                    "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
        stack_dump(stk);

#endif //STACK_H