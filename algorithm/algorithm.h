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
*
*   @param a          [in] - первое число
*   @param b          [in] - второе число
*   @param error_rate [in] - погрешность сравнения
*
*   @return 0, если a = b с учётом погрешности error_rate; < 0, если a < b; > 0, если a > b
*/
int dblcmp(const double a, const double b, const double error_rate = DELTA);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Обменивает значения двух переменных.
*
*   @param a         [in, out] - указатель на первую переменную
*   @param b         [in, out] - указатель на вторую переменную
*   @param elem_size [in]      - размер (в байтах) переменной
*/
void my_swap(void *a, void *b, size_t elem_size);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Сравнивает (по байтам) две переменные.
*
*   @param a         [in] - указатель на первую переменную
*   @param b         [in] - указатель на вторую переменную
*   @param elem_size [in] - размер (в байтах) переменной
*
*   @return true, если переменные равны, false иначе
*
*/
bool is_byte_equal(const void *a, const void *b, size_t elem_size);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Считывает space-символы до первого не space-символа.
*
*   @param stream [in] - поток
*
*   @return EOF, если после space-символов встретился конец файла, и 0 иначе
*/
int skip_spaces(FILE *const stream);

//--------------------------------------------------------------------------------------------------------------------------------

#define num_cmp(type, name, operator)                                                       \
__always_inline type name(const type a, const type b);                                      \
                type name(const type a, const type b) { return (a operator b) ? a : b; }

num_cmp(int      , int_min, <)
num_cmp(int      , int_max, >)
num_cmp(long     ,   l_min, <)
num_cmp(long     ,   l_max, >)
num_cmp(long long,  ll_min, <)
num_cmp(long long,  ll_max, >)

num_cmp(size_t            , size_t_min, <)
num_cmp(size_t            , size_t_max, >)
num_cmp(unsigned          ,   uint_min, <)
num_cmp(unsigned          ,   uint_max, >)
num_cmp(unsigned long     ,     ul_min, <)
num_cmp(unsigned long     ,     ul_max, >)
num_cmp(unsigned long long,    ull_min, <)
num_cmp(unsigned long long,    ull_max, >)

#undef num_cmp

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выделяет динамическую память для буфера buff размера buff_size.
*
*   @param buff      [out] - буфер
*   @param buff_size [in]  - размер буфера (в байтах)
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool buffer_ctor(buffer *const buff, const size_t buff_size);

/**
*   @brief Выделяет динамическую память для содержимого файла <file_name>, заполняет поля буфера buff.
*
*   @param buff      [out] - буфер
*   @param file_name [in]  - имя файла
*
*   @return true если всё ОК, false в случае ошибки
*/
bool buffer_ctor(buffer *const buff, const char *const file_name);

/**
*   @brief Buffer_dtor.
*
*   @param _buff [in] - буффер
*/
void buffer_dtor(void *const _buff);

/**
*   @brief Buffer_dump.
*
*   @param _buff [in] - буффер
*/
void buffer_dump(const void *const _buff);

#endif //ALGORITHM_H