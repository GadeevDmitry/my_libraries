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
*   @brief Сравнивает два вещественных числа.
*   Оболочка для back_trace.
*
*   @param cur_file   [in] - файл в точке вызова
*   @param cur_func   [in] - функция в точке вызова
*   @param cur_line   [in] - строка в точке вызова
*
*   @param a          [in] - первое число
*   @param b          [in] - второе число
*   @param error_rate [in] - погрешность сравнения
*
*   @return   0, если a = b с учётом погрешности error_rate; < 0, если a < b; > 0, если a > b
*/
int _dblcmp(const char *const cur_file,
            const char *const cur_func,
            const int         cur_line,

            const double a, const double b, const double error_rate = DELTA);

/**
*   @brief Обменивает значения двух переменных.
*   Оболочка для back_trace.
*
*   @param cur_file  [in]      - файл в точке вызова
*   @param cur_func  [in]      - функция в точке вызова
*   @param cur_line  [in]      - строка в точке вызова
*
*   @param a         [in, out] - указатель на первую переменную
*   @param b         [in, out] - указатель на вторую переменную
*   @param elem_size [in]      - размер (в байтах) переменной
*/
void _my_swap(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              void *a, void *b, size_t elem_size);

/**
*   @brief Сравнивает (по байтам) две переменные.
*   Оболочка для back_trace.
*
*   @param cur_file  [in] - файл в точке вызова
*   @param cur_func  [in] - функция в точке вызова
*   @param cur_line  [in] - строка в точке вызова
*
*   @param a         [in] - указатель на первую переменную
*   @param b         [in] - указатель на вторую переменную
*   @param elem_size [in] - размер (в байтах) переменной
*
*   @return true, если переменные равны, false иначе
*
*/
bool _is_byte_equal(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    const void *a, const void *b, size_t elem_size);

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выделяет динамическую память для буфера buff размера buff_size.
*   Оболочка для back_trace.
*
*   @param cur_file  [in]  - файл в точке вызова
*   @param cur_func  [in]  - функция в точке вызова
*   @param cur_line  [in]  - строка в точке вызова
*
*   @param buff      [out] - буфер
*   @param buff_size [in]  - размер буфера (в байтах)
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool _buffer_ctor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  buffer *const buff, const size_t buff_size);

/**
*   @brief Выделяет динамическую память для содержимого файла <file_name>, заполняет поля буфера buff.
*   Оболочка для back_trace.
*
*   @param cur_file  [in]  - файл в точке вызова
*   @param cur_func  [in]  - функция в точке вызова
*   @param cur_line  [in]  - строка в точке вызова
*
*   @param buff      [out] - буфер
*   @param file_name [in]  - имя файла
*
*   @return true если всё ОК, false в случае ошибки
*/
bool _buffer_ctor_file(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line,

                       buffer *const buff, const char *const file_name);

/**
*   @brief Buffer_dtor.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param _buff    [in] - буффер
*/
void _buffer_dtor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  void *const _buff);

/**
*   @brief Buffer_dump.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param _buff    [in] - буффер
*/
void _buffer_dump(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const void *const _buff);

#include "algorithm_def.h"

#endif //ALGORITHM_H