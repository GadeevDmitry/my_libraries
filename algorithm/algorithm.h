/** @file */
#ifndef ALGORITHM_H
#define ALGORITHM_H

//================================================================================================================================
// GLOBAL
//================================================================================================================================

#include "../logs/log.h"
const double DELTA = 0.0001; ///< погрешность сравнения вещественных чисел по умолчанию

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

#define poison_field_dump(field)                         log_tab_message(field " = " HTML_COLOR_POISON      "POISON"       HTML_COLOR_CANCEL "\n")

#define good_field_dump(   field, specified_char, value) log_tab_message(field " = " HTML_COLOR_LIME_GREEN  specified_char HTML_COLOR_CANCEL "\n", value)
#define warning_field_dump(field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_ORANGE specified_char HTML_COLOR_CANCEL "\n", value)
#define error_field_dump(  field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_RED    specified_char HTML_COLOR_CANCEL "\n", value)
#define usual_field_dump(  field, specified_char, value) log_tab_message(field " = "                        specified_char                   "\n", value)

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура для хранения массива символов
*/
struct buffer
{
    char  *buff_beg;    ///< указатель на начало буфера
    char  *buff_pos;    ///< указатель на текущую позицию
    size_t buff_size;   ///< размер буфера
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// USEFUL FUNCTION
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для static int dblcmp(const double, const double, const double) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see dblcmp(const double, const double, const double)
*/
int _dblcmp(const char *const cur_file,
            const char *const cur_func,
            const int         cur_line,

            const double a, const double b, const double error_rate = DELTA);

/**
*   @brief Оболочка для static void my_swap(void *, void *, size_t) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see my_swap(void*, void*, size_t)
*/
void _my_swap(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              void *a, void *b, size_t elem_size);

/**
*   @brief Оболочка для static bool is_byte_equal(const void *, const void *, size_t) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see is_byte_equal(const void*, const void*, size_t)
*/
bool _is_byte_equal(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    const void *a, const void *b, size_t elem_size);

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для static bool buffer_ctor(buffer *, const size_t) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see buffer_ctor(buffer*, const size_t)
*/
bool _buffer_ctor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  buffer *const buff, const size_t buff_size);

/**
*   @brief Оболочка для static bool buffer_ctor_file(buffer *, const char *) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see buffer_ctor_file(buffer*, const char*)
*/
bool _buffer_ctor_file(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line,

                       buffer *const buff, const char *const file_name);

/**
*   @brief Оболочка для static void buffer_dtor(void *) для trace_push и trace_pop
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see buffer_dtor(void*)
*/
void _buffer_dtor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  void *const _buff);

/**
*   @brief Оболочка для static void _buffer_dump(const void *) для trace_push и trace_pop
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @see _buffer_dump(const void*)
*/
void _buffer_dump(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const void *const _buff);

#include "algorithm_def.h"

#endif //ALGORITHM_H