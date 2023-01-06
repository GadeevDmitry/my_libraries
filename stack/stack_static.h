/** @file */
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

#include "stack_settings.h"
#include "stack.h"

#undef stack_dump

#ifdef STACK_CANARY_PROTECTION
typedef unsigned long long stk_canary_t;
#endif
#ifdef STACK_HASH_PROTECTION
typedef unsigned long long stk_hash_t;
#endif

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// HTML_COLOR
//--------------------------------------------------------------------------------------------------------------------------------

#define HTML_COLOR_POISON HTML_COLOR_OLIVE

//--------------------------------------------------------------------------------------------------------------------------------
// STACK
//--------------------------------------------------------------------------------------------------------------------------------

#define $data     stk->data

#define $el_size  stk->el_size
#define $size     stk->size
#define $capacity stk->capacity

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

    STK_POISON_DATA                 , ///< .data     = STK_POISON.data
    STK_POISON_EL_SIZE              , ///< .el_size  = STK_POISON.el_size
    STK_POISON_SIZE                 , ///< .size     = STK_POISON.size
    STK_POISON_CAPACITY             , ///< .capacity = STK_POISON.capacity

    STK_INVALID_SIZE_CAPACITY       , ///< .size > .capacity

    #ifdef STACK_CANARY_PROTECTION
    STK_L_CANARY_FAILED             , ///< невалидное значение левой канарейки
    STK_R_CANARY_FAILED             , ///< невалидное значение правой канарейки
    #endif
    #ifdef STACK_HASH_PROTECTION
    STK_HASH_FAILED                 , ///< невалидное значение хэша
    #endif
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
    "stack.el_size is invalid"              ,

    "stack.size more than stack.capacity"   ,

    #ifdef STACK_CANARY_PROTECTION
    "left stack canary protection faled"    ,
    "right stack canary protection failed"  ,
    #endif
    #ifdef STACK_HASH_PROTECTION
    "stack hash protection failed"          ,
    #endif
};

/**
*   @brief POISON-значения полей стека
*/
static const stack STK_POISON =
{
    (void *) 0xBADF00D  , //data

    0xABADBABE          , //el_size
    0xBADCAB1E          , //size
    0xDEADBEFF          , //capacity
};

#ifdef STACK_CANARY_PROTECTION
static const stk_canary_t L_CANARY = 0xDEADBABE; ///< валидное значение левой канарейки
static const stk_canary_t R_CANARY = 0xCAFEBABE; ///< валидное значение правой канарейки
#endif
#ifdef STACK_HASH_PROTECTION
static const stk_hash_t   HASH_BEG = 0xFEE1DEAD; ///< начальный хэш (хэш пустого стека)
#endif

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack canary
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef STACK_CANARY_PROTECTION

/**
*   @brief Находит значение левой канарейки в стеке stk
*
*   @see static void stk_set_left_canary(stack *const stk, const stk_canary_t value)
*   @see static stk_canary_t stk_get_right_canary(const stack *const stk)
*   @see static void stk_set_right_canary(stack *const stk, const stk_canary_t value)
*/
static stk_canary_t stk_get_left_canary(const stack *const stk);

/**
*   @brief Устанавливает значение левой канарейки стека stk равным value
*
*   @see static stk_canary_t stk_get_left_canary(const stack *const stk)
*   @see static stk_canary_t stk_get_right_canary(const stack *const stk)
*   @see static void stk_set_right_canary(stack *const stk, const stk_canary_t value)
*/
static void stk_set_left_canary(stack *const stk, const stk_canary_t value);

/**
*   @brief Находит значение правой канарейки стека stk
*
*   @see static stk_canary_t stk_get_left_canary(const stack *const stk)
*   @see static void stk_set_left_canary(stack *const stk, const stk_canary_t value)
*   @see static void stk_set_right_canary(stack *const stk, const stk_canary_t value)
*/
static stk_canary_t stk_get_right_canary(const stack *const stk);

/**
*   @brief Устанавливает значение правой канарейки стека stk равным value
*
*   @see static stk_canary_t stk_get_left_canary(const stack *const stk)
*   @see static void stk_set_left_canary(stack *const stk, const stk_canary_t value)
*   @see static stk_canary_t stk_get_right_canary(const stack *const stk)
*/
static void stk_set_right_canary(stack *const stk, const stk_canary_t value);

#endif //STACK_CANARY_PROTECTION

//--------------------------------------------------------------------------------------------------------------------------------
// stack hash
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef STACK_HASH_PROTECTION

/**
*   @brief Вычисляет актуальный хэш стека stk
*
*   @see static stk_hash_t stk_get_saved_hash(const stack *const stk)
*   @see static void stk_set_hash(stack *const stk)
*/
static stk_hash_t stk_get_actual_hash(const stack *const stk);

/**
*   @brief Возвращает значение хэша, лежащего в памяти (должно совпадать с актуальным хэшом, если стек валидный)
*
*   @see static stk_hash_t stk_get_actual_hash(const stack *const stk)
*   @see static void stk_set_hash(stack *const stk)
*/
static stk_hash_t stk_get_saved_hash(const stack *const stk);

/**
*   @brief Вычисляет актуальный хэш стека и устанавливает его
*
*   @see static stk_hash_t stk_get_actual_hash(const stack *const stk)
*   @see static stk_hash_t stk_get_saved_hash(const stack *const stk)
*/
static void stk_set_hash(stack *const stk);

#endif //STACK_HASH_PROTECTION

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор стека
*
*   @param stk  [in] - стек для верификатора
*   @param file [in] - файл в точке вызова
*   @param func [in] - функция в точке вызова
*   @param line [in] - строка в точке вызова
*
*   @return битовую маску кодов ошибки из enum STK_STATUS
*
*   @see enum STK_STATUS
*/
static unsigned stack_verify(const stack *const stk,    const char *file,
                                                        const char *func,
                                                        const int   line);


//--------------------------------------------------------------------------------------------------------------------------------
// stack static dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщение об ошибке в стеке в лог
*/
static void stack_log_error(const stack *const stk, const unsigned stk_error,   const char *file,
                                                                                const char *func,
                                                                                const int   line);

/**
*   @brief Полный dump стека (включая канарейки, хэш, всю емкость стека)
*
*   @param stk  [in] - stack to dump
*   @param file [in] - имя файла, в котором произошла ошибка
*   @param func [in] - имя функции, в которой произошла ошибка
*   @param line [in] - номер строки, в которой произошла ошибка
*
*   @see static void stack_public_fields_dump(const stack *const stk)
*/
static void stack_static_dump(const stack *const stk, const char *file, const char *func, const int line);

/**
*   @brief Dump public-полей стека
*
*   @see static void stack_static_dump(const stack *const stk)
*/
static void stack_public_fields_dump(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack resize
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Resize стека stk
*   При добавлении элемента в стек:
*   @code
*       if (stk.size == stk.capacity) stk.capacity = 2 * stk.capacity;
*       PUSH
*   @endcode
*   При удалении элемента из стека:
*   @code
*       POP
*       if (4 * stk.size <= stk.capacity) stk.capacity = max(DEFAULT_STACK_CAPACITY, stk.capacity / 2);
*   @endcode
*/
static bool stack_resize(stack *const stk, const size_t new_capacity);

#endif //STACK_STATIC_H