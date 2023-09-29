/** @file */
#ifndef ALGORITHM_STATIC_H
#define ALGORITHM_STATIC_H

#include <math.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

//#define NDEBUG
//#define NVERIFY

#include "algorithm.h"
#include "log.h"

//================================================================================================================================

/**
*   @brief Типы ошибок в буфере.
*/
typedef enum
{
    BUFF_OK             ,   ///< OK
    BUFF_NULLPTR        ,   ///< buff = nullptr

    BUFF_POISON_BEG     ,   ///< .beg  = BUFF_POISON.beg
    BUFF_POISON_POS     ,   ///< .pos  = BUFF_POISON.pos
    BUFF_POISON_SIZE    ,   ///< .size = BUFF_POISON.size

    BUFF_BEG_NULLPTR    ,   ///< .beg = nullptr
    BUFF_POS_NULLPTR    ,   ///< .pos = nullptr

    BUFF_POS_LESS_BEG   ,   ///< .pos <  .beg
    BUFF_POS_MORE_END   ,   ///< .pos >= .beg + .size
}
BUFF_STATUS_TYPE;

/**
*   @brief Сообщения для каждого типа ошибок.
*
*   @see enum BUFF_STATUS_TYPE
*/
static const char *BUFF_STATUS_MESSAGES[] =
{
    "buffer is ok"                          ,
    "buffer is nullptr"                     ,

    "buffer.beg  is invalid"                ,
    "buffer.pos  is invalid"                ,
    "buffer.size is invalid"                ,

    "buffer.beg is nullptr"                 ,
    "buffer.pos is nullptr"                 ,

    "buffer.pos is less than buffer.beg"    ,
    "buffer.pos is more thna buffer end"    ,
};

/**
*   @brief POISON-значения полей буфера.
*/
static const buffer BUFF_POISON =
{
    .beg  = (char *) 0xDEADBEEF,
    .pos  = (char *) 0xABADBABE,
    .size =          0xABADB002,
};

//================================================================================================================================

static void     buffer_log_error          (const buffer *const buff, const unsigned err);
static unsigned buffer_fields_verify      (const buffer *const buff);

static bool     get_file_size             (const char *file_name, size_t *const file_size);

static void     buffer_static_dump        (const buffer *const buff, const bool is_full);
static bool     buffer_header_dump        (const buffer *const buff);
static bool     buffer_public_fields_dump (const buffer *const buff);
static bool     buffer_static_fields_dump (const buffer *const buff);
static void     buffer_content_dump       (const buffer *const buff, const bool is_any_invalid);
static void     buffer_content_safety_dump(const buffer *const buff);
static void     buffer_hex_content_dump   (const buffer *const buff);

//================================================================================================================================

const double DELTA = 0.0001;

#endif //ALGORITHM_STATIC_H
