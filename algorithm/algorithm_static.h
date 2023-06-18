/** @file */
#ifndef ALGORITHM_STATIC_H
#define ALGORITHM_STATIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "algorithm_settings.h"
#include "algorithm.h"

//================================================================================================================================
// DSL
//================================================================================================================================

#define $buff_beg   (buff->buff_beg)
#define $buff_pos   (buff->buff_pos)
#define $buff_size  (buff->buff_size)

//================================================================================================================================
// GLOBAL
//================================================================================================================================

/**
*   @brief Типы ошибок в буфере.
*/
enum BUFF_STATUS
{
    BUFF_OK             ,
    BUFF_NULLPTR        ,

    BUFF_POISON_BEG     ,
    BUFF_POISON_POS     ,
    BUFF_POISON_SIZE    ,

    BUFF_BEG_NULLPTR    ,
    BUFF_POS_NULLPTR    ,

    BUFF_POS_LESS_BEG   ,
    BUFF_POS_MORE_END   ,
};

/**
*   @brief Сообщения для каждого типа ошибок
*
*   @see enum BUFF_STATUS
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
*   @brief POISON-значения полей стека
*/
static const buffer BUFF_POISON =
{
    (char *) 0xDEADBEEF ,   // buff_beg
    (char *) 0xABADBABE ,   // buff_pos
             0xABADB002 ,   // buff_size
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// buffer verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщения об ошибках в буфере по битовой маске ошибок. Полный дамп буфера.
*/
static void buff_log_error(const buffer *const buff, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет на наличие POISON-полей в буфере.
*
*   @return битовая маска кодов ошибок из enum BUFF_STATUS
*
*   @see enum BUFF_STATUS
*/
static unsigned buff_poison_verify(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор полей буфера.
*
*   @return битовая маска кодов ошибок из enum BUFF_STATUS
*
*   @see enum BUFF_STATUS
*/
static unsigned buff_fields_verify(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------
// buffer ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Находит размер файла <file_name>.
*
*   @param file_name [in]  - имя файла
*   @param file_size [out] - размер файла
*
*   @return true, если всё ОК, false в случае ошибки
*/
static bool get_file_size(const char *file_name, size_t *const file_size);

//--------------------------------------------------------------------------------------------------------------------------------
// buffer dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп буфера.
*
*   @param buff    [in] - указатель на буфер
*   @param is_full [in] - true, если нужен дамп "static" полей
*/
static void buffer_static_dump(const buffer *const buff, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Шапка" дампа буфера.
*
*   @return true, если buff != nullptr
*/
static bool buffer_header_dump(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конец дампа буфера.
*
*   @return true, если buff != nullptr
*/
static void buffer_ending_dump();

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп "public" полей буфера.
*
*   @return true, если хоть одно поле невалидно
*/
bool buffer_public_fields_dump(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп "static" полей буфера.
*
*   @return true, если хоть одно поле невалидно
*/
bool buffer_static_fields_dump(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого буфера.
*
*   @param buff           [in] - указатель на буфер
*   @param is_any_invalid [in] - true, если хоть одно поле невалидно
*/
static void buffer_content_dump(const buffer *const buff, const bool is_any_invalid);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого буфера при гарантии, что все поля валидны.
*/
static void buffer_content_safety_dump(const buffer *const buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого буфера в виде последовательности hex-байтов при гарантии, что все поля валидны.
*/
static void buffer_hex_content_dump(const buffer *const buff);

#endif //ALGORITHM_STATIC_H
