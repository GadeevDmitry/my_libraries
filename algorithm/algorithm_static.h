/** @file */
#ifndef ALGORITHM_STATIC_H
#define ALGORITHM_STATIC_H

#include "algorithm.h"
#include "algorithm_undef.h"

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// USEFUL FUNCTION
//--------------------------------------------------------------------------------------------------------------------------------

static int _dblcmp        (const double a, const double b, const double error_rate = DELTA);
static void _my_swap      (       void *a,        void *b,       size_t elem_size         );
static bool _is_byte_equal(const  void *a, const  void *b,       size_t elem_size         );

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------


static bool _buffer_ctor     (buffer *const buff, const size_t      buff_size);
static bool _buffer_ctor_file(buffer *const buff, const char *const file_name);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _get_file_size(const char *file_name, size_t *const file_size);

/**
*   @brief Находит размер файла <file_name>
*
*   @param cur_file  [in]  - файл в точке вызова
*   @param cur_func  [in]  - функция в точке вызова
*   @param cur_line  [in]  - строка в точке вызова
*
*   @param file_name [in]  - имя файла
*   @param file_size [out] - размер файла
*
*   @return true, если всё ОК, false в случае ошибки
*/
static bool _get_file_size(const char *const cur_file,
                           const char *const cur_func,
                           const int         cur_line,

                           const char *const file_name, size_t *const file_size);

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_dtor(void *const _buff);

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_dump(const void *const _buff);

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_content_dump(const buffer *const buff);

/**
*   @brief Dump содержимого буффера.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param buff     [in] - буффер
*/
static void _buffer_content_dump(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const buffer *const buff);

//================================================================================================================================
// MACRO DEFINITIONS
//================================================================================================================================

#define get_file_size(file_name, file_size) _get_file_size      (__FILE__, __PRETTY_FUNCTION__, __LINE__, file_name, file_size)
#define buffer_content_dump(buff)           _buffer_content_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__,                 buff)

#endif //ALGORITHM_STATIC_H