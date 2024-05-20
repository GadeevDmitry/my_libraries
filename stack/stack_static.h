/** @file */
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "log.h"
#include "stack.h"

//================================================================================================================================

const size_t DEFAULT_STACK_CAPACITY = 4;

//================================================================================================================================

/**
*   @brief Типы ошибок в стеке.
*/
typedef enum
{
    STK_OK                          , ///< ОК
    STK_NULLPTR                     , ///< stk = nullptr
    STK_NULLPTR_DATA                , ///< .data = nullptr

    STK_POISON_DATA                 , ///< .data      = STK_POISON.data
    STK_POISON_EL_SIZE              , ///< .el_size   = STK_POISON.el_size
    STK_POISON_SIZE                 , ///< .size      = STK_POISON.size
    STK_POISON_CAPACITY             , ///< .capacity  = STK_POISON.capacity
    STK_POISON_EL_POISON            , ///< .el_poison = STK_POISON.el_poison
    STK_POISON_EL_DTOR              , ///< .el_dtor   = STK_POISON.el_dtor
    STK_POISON_EL_DUMP              , ///< .el_dump   = STK_POISON.el_dump

    STK_NOT_POISON_GAP              , ///< не POISON-значение в неиспользуемой ячейке
    STK_INVALID_SIZE_CAPACITY       , ///< .size > .capacity
}
STK_STATUS_TYPE;

/**
*   @brief Сообщения для каждого типа ошибок.
*
*   @see enum STK_STATUS_TYPE
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

    "stack gap is not poisoned"            ,
    "stack.size more than stack.capacity"  ,
};

static const stack STK_POISON =
{
    .data = (void *) 0xBADF00D,

    .el_size  = 0xABADBABE,
    .size     = 0xBADCAB1E,
    .capacity = 0xDEADBEFF,

    .el_poison = (const void *) 0xCAFED00D,

    .el_dtor = (void (*)(      void *const)) 0xFEEDFACE,
    .el_dump = (void (*)(const void *const)) 0xDEADDEAD,
};

//================================================================================================================================

static void     stack_gap_fill_poison   (stack *const stk);
static void     stack_el_fill_poison    (stack *const stk, const size_t filled_index);

static bool     stack_gap_is_poison     (const stack *const stk);
static bool     stack_el_is_poison      (const stack *const stk, const size_t check_index);

static void    *stack_get               (const stack *const stk, const size_t index);

static void     stack_log_error         (const stack *const stk, const unsigned err);
static unsigned stack_fields_verify     (const stack *const stk);

static void     stack_data_dtor         (stack *const stk);
static bool     stack_resize            (stack *const stk, const size_t new_capacity);

static void     stack_static_dump       (const stack *const stk, const bool is_full);
static bool     stack_header_dump       (const stack *const stk);
static bool     stack_public_fields_dump(const stack *const stk);
static bool     stack_static_fields_dump(const stack *const stk);
static void     stack_data_dump         (const stack *const stk, const bool is_full, const bool are_invalid_fields);
static void     stack_data_debug_dump   (const stack *const stk);
static void     stack_data_pretty_dump  (const stack *const stk);
static void     stack_el_dump           (const stack *const stk, const void *const el);

#endif // STACK_STATIC_H
