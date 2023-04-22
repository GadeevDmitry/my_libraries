/** @file */
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

#include "stack_settings.h"
#include "stack.h"

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// STACK
//--------------------------------------------------------------------------------------------------------------------------------

#define $data           (stk->data)

#define $el_size        (stk->el_size)
#define $size           (stk->size)
#define $capacity       (stk->capacity)

#define $el_poison      (stk->el_poison)

#define $el_dtor        (stk->el_dtor)
#define $el_dump        (stk->el_dump)

//================================================================================================================================
// GLOBAL
//================================================================================================================================

/**
*   @brief Типы ошибок в стеке
*/
enum STK_STATUS
{
    STK_OK                          , ///< ОК
    STK_NULLPTR                     , ///< stk = nullptr
    STK_NULLPTR_DATA                , ///< .data = nullptr

    STK_POISON_DATA                 , ///< .data         = STK_POISON.data
    STK_POISON_EL_SIZE              , ///< .el_size      = STK_POISON.el_size
    STK_POISON_SIZE                 , ///< .size         = STK_POISON.size
    STK_POISON_CAPACITY             , ///< .capacity     = STK_POISON.capacity
    STK_POISON_EL_POISON            , ///< .el_poison    = STK_POISON.el_poison
    STK_POISON_EL_DTOR              , ///< .el_dtor      = STK_POISON.el_dtor
    STK_POISON_EL_DUMP              , ///< .el_dump      = STK_POISON.el_dump

    STK_NOT_POISON_GAP              , ///< не POISON-значение в неиспользуемой ячейке
    STK_INVALID_SIZE_CAPACITY       , ///< .size > .capacity
};

/**
*   @brief Сообщения для каждого типа ошибок
*
*   @see enum STK_STATUS
*/
static const char *STK_STATUS_MESSAGES[] =
{
    "stack is ok"                           ,
    "stack is nullptr"                      ,
    "stack.data is nullptr"                 ,

    "stack.data is invalid"                 ,
    "stack.size is invalid"                 ,
    "stack.capacity is invalid"             ,
    "stack.el_poison is invalid"            ,
    "stack.el_size is invalid"              ,
    "stack.el_dtor is invalid"              ,
    "stack.el_dump is invalid"              ,

    "stack gap is not poisoned"             ,
    "stack.size more than stack.capacity"   ,
};

/**
*   @brief POISON-значения полей стека
*/
static const stack STK_POISON =
{
    (void *) 0xBADF00D                      , //data

    0xABADBABE                              , //el_size
    0xBADCAB1E                              , //size
    0xDEADBEFF                              , //capacity

    (const void *) 0xCAFED00D               , //el_poison

    (void (*)(      void *const)) 0xFEEDFACE, //el_dtor
    (void (*)(const void *const)) 0xDEADDEAD, //el_dump
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Устанавливает POISON-значения в неиспользуемые ячейки стека.
*/
static void stack_gap_fill_poison(stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Заполняет POISON-значения в элемент стека.
*/
static void stack_el_fill_poison(stack *const stk, const size_t filled_index);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет наличие POISON-значений в неиспользуемых ячейках стека.
*/
static bool stack_gap_is_poison(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет наличие POISON-значений в элементе стека.
*/
static bool stack_el_is_poison(const stack *const stk, const size_t check_index);

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает указатель на элемент стека.
*/
static void *stack_get(const stack *const stk, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор стека.
*
*   @return битовая маска кодов ошибок из enum STK_STATUS
*
*   @see enum STK_STATUS
*/
static unsigned _stack_verify(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщения об ошибках в стеке в лог.
*/
static void stack_log_error(const stack *const stk, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Dtor элементов стека.
*/
static void stack_data_dtor(stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Реаллоцирует стек.
*/
static bool stack_resize(stack *const stk, const size_t new_capacity);

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп стека.
*
*   @param stk     [in] - стек
*   @param is_full [in] - true, если полный (служебный) дамп, false иначе
*/
static void stack_static_dump(const stack *const stk, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп полей стека.
*/
static void stack_public_fields_dump(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого стека.
*/
static void stack_data_dump(const stack *const stk, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп элемента стека.
*/
static void stack_el_dump(const stack *const stk, const void *const el);

//================================================================================================================================
// MACRO DEFINITIONS
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

#if !defined(NVERIFY) && !defined(STACK_NVERIFY)
#define stack_verify(stk, ret_val)              \
    if (_stack_verify(stk) != STK_OK)           \
    {                                           \
    $o  return ret_val;                         \
    }
#else
#define stack_verify(stk, ret_val)
#endif

#if !defined(NDEBUG) && !defined(STACK_NDEBUG)
#define stack_debug_verify(stk)                 \
        log_assert(_stack_verify(stk) == STK_OK)
#else
#define stack_debug_verify(stk)
#endif

#endif //STACK_STATIC_H
