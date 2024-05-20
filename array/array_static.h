/** @file */
#ifndef ARRAY_STATIC_H
#define ARRAY_STATIC_H

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "array.h"

//================================================================================================================================

#define ARRAY_GET(arr, ind) (char *) arr->data + arr->el_size * ind

//================================================================================================================================

/**
*   @brief Типы ошибок в массиве.
*/
typedef enum
{
    ARR_OK                  ,   ///< OK
    ARR_NULLPTR             ,   ///< arr = nullptr

    ARR_NULLPTR_DATA        ,   ///< .data    = nullptr
    ARR_ZERO_SIZE           ,   ///< .size    = 0
    ARR_ZERO_EL_SIZE        ,   ///< .el_size = 0

    ARR_POISON_DATA         ,   ///< .data    = LST_POISON.data
    ARR_POISON_SIZE         ,   ///< .size    = LST_POISON.size
    ARR_POISON_EL_SIZE      ,   ///< .el_size = LST_POISON.el_size
    ARR_POISON_EL_DTOR      ,   ///< .el_dtor = LST_POISON.el_dtor
    ARR_POISON_EL_DUMP      ,   ///< .el_dump = LST_POISON.el_dump
}
ARR_STATUS_TYPE;

/**
*   @brief Сообщения для каждого типа ошибок.
*
*   @see enum ARR_STATUS_TYPE
*/
static const char *ARR_STATUS_MESSAGES[] =
{
    "array is ok"                   ,
    "array is nullptr"              ,

    "array.data is nullptr"         ,
    "array.size is zero"            ,
    "array.el_size is zero"         ,

    "array.data is invalid"         ,
    "array.size is invalid"         ,
    "array.el_size is invalid"      ,
    "array.el_dtor is invalid"      ,
    "array.el_dump is invalid"      ,
};

/**
*   @brief POISON-значения полей массива.
*/
static const array ARR_POISON
{
    .data    = (void *) 0xABADF00D,
    .size    =          0xBADCAB1E,
    .el_size =          0xABADBABE,

    .el_dtor = (void (*) (      void *const)) 0xFEEDFACE,
    .el_dump = (void (*) (const void *const)) 0xDEADDEAD,
};

//================================================================================================================================

static void     array_log_error         (const array *const arr, const unsigned err);
static unsigned array_fields_verify     (const array *const arr);

static void     array_data_dtor         (array *const arr);

static void     array_static_dump       (const array *const arr, const bool is_full);
static bool     array_header_dump       (const array *const arr);
static bool     array_public_fields_dump(const array *const arr);
static bool     array_static_fields_dump(const array *const arr);
static void     array_data_dump         (const array *const arr, const bool is_any_invalid);
static void     array_data_content_dump (const array *const arr);

#endif // ARRAY_STATIC_H
