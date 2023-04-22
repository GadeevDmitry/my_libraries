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

/**
*   @brief Stack_ctor.
*
*   @param stk       [out] - указатель на стек
*   @param el_size   [in]  - размер элемента стека
*   @param el_poison [in]  - указатель на POISON-элемент стека
*   @param el_dtor   [in]  - указатель на dtor элемента стека
*   @param el_dump   [in]  - указатель на dump элемента стека
*/
bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     = nullptr,
                                                              void (     *el_dtor  )(      void *const) = nullptr,
                                                              void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает стек в динамической памяти.
*
*   @param el_size   [in]  - размер элемента стека
*   @param el_poison [in]  - указатель на POISON-элемент стека
*   @param el_dtor   [in]  - указатель на dtor элемента стека
*   @param el_dump   [in]  - указатель на dump элемента стека
*
*   @return указатель на созданный стек или nullptr в случае ошибки
*/
stack *stack_new(const size_t el_size, const void *const el_poison                     = nullptr,
                                             void (     *el_dtor  )(      void *const) = nullptr,
                                             void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack_dtor.
*
*   @param stk      [in] - указатель на стек
*/
void stack_dtor(void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Вызывает stack_dtor(void* _stk) и освобождает динамическую память, на которую должен указывать стек.
*
*   @see stack_dtor(void* _stk)
*/
void stack_free(void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Кладет элемент стека в конец.
*
*   @param stk      [in, out] - стек
*   @param data     [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool stack_push(stack *const stk, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет последний элемент стека.
*
*   @param stk      [in, out] - стек
*   @param data     [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool stack_pop(stack *const stk, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое последнего элемента стека.
*
*   @param stk      [in]  - стек
*   @param data     [out] - указатель, куда скопировать содержимое удаляемого элемента
*
*   @return true, если все ОК, false в случае ошибки
*/
bool stack_front(const stack *const stk, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет, пустой ли стек.
*
*   @param stk      [in] - стек
*
*   @return true, если стек пустой, false иначе, или в случае ошибки
*/
bool stack_is_empty(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack_dump.
*
*   @param _stk     [in] - стек
*/
void stack_dump(const void *const _stk);

#endif //STACK_H