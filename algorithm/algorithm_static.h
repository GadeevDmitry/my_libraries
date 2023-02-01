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

/**
*   @brief Сравнивает два вещественных числа
*
*   @param a          [in] - первое число
*   @param b          [in] - второе число
*   @param error_rate [in] - погрешность сравнения
*
*   @return   0, если a = b с учётом погрешности error_rate
*   @return < 0, если a < b
*   @return > 0, если a > b
*/
static int _dblcmp(const double a, const double b, const double error_rate = DELTA);

/**
*   @brief Обменивает значения двух переменных
*
*   @param a         [in] - указатель на первую переменную
*   @param b         [in] - указатель на вторую переменную
*   @param elem_size [in] - размер (в байтах) переменной
*/
static void _my_swap(void *a, void *b, size_t elem_size);

/**
*   @brief Сравнивает (по байтам) две переменные
*
*   @param a         [in] - указатель на первую переменную
*   @param b         [in] - указатель на вторую переменную
*   @param elem_size [in] - размер (в байтах) переменной
*
*   @return true, если переменные равны, false иначе
*
*/
static bool _is_byte_equal(const void *a, const void *b, size_t elem_size);

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выделяет динамическую память для буфера buff размера buff_size
*
*   @param buff      [out] - буфер
*   @param buff_size [in]  - размер буфера (в байтах)
*
*   @return true, если всё ОК
*   @return false в случае ошибки
*
*   @see buffer_ctor_file(buffer*, const char*)
*/
static bool _buffer_ctor(buffer *const buff, const size_t buff_size);

/**
*   @brief Выделяет динамическую память для содержимого файла <file_name>, заполняет поля буфера buff
*
*   @param buff      [out] - буфер
*   @param file_name [in]  - имя файла
*
*   @return true если всё ОК
*   @return false в случае ошибки
*
*   @see buffer_ctor(buffer*, const size_t)
*/
static bool _buffer_ctor_file(buffer *const buff, const char *const file_name);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Находит размер файла <file_name>
*
*   @param file_name [in]  - имя файла
*   @param file_size [out] - размер файла
*
*   @return true, если всё ОК
*   @return false в случае ошибки
*/
static bool _get_file_size(const char *file_name, size_t *const file_size);

/**
*   @brief Оболочка для static bool get_file_size(const char *, size_t *) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see get_file_size(const char*, size_t*)
*/
static bool _get_file_size(const char *const cur_file,
                           const char *const cur_func,
                           const int         cur_line,

                           const char *const file_name, size_t *const file_size);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Деструктор буфера
*/
static void _buffer_dtor(void *const _buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп буффера
*/
static void _buffer_dump(const void *const _buff);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого буффера
*/
static void _buffer_content_dump(const buffer *const buff);

/**
*   @brief Оболочка для static void _buffer_content_dump(const buffer *) для trace_push и trace_pop
*
*   @see _buffer_content_dump(const buffer*)
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