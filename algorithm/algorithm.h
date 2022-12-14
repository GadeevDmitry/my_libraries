/** @file */
#ifndef ALGORITHM_H
#define ALGORITHM_H

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// GLOBAL
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

const double DELTA = 0.0001; ///< погрешность сравнения вещественных чисел по умолчанию

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// STRUCT
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
*   @brief Структура для хранения массива символов
*/
struct buffer
{
    char  *buff_beg;    ///< указатель на начало буфера
    char  *buff_pos;    ///< указатель на текущую позицию
    size_t buff_size;   ///< размер буфера
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// FUNCTION DECLARATION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//================================================================================================================================
// USEFUL FUNCTION
//================================================================================================================================

/**
*   @brief Обменивает значения двух переменных
*
*   @param a         [in] - указатель на первую переменную
*   @param b         [in] - указатель на вторую переменную
*   @param elem_size [in] - размер (в байтах) переменной
*/
void my_swap(void *a, void *b, size_t elem_size);

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
int dblcmp(const double a, const double b, const double error_rate = DELTA);

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
bool is_byte_equal(const void *a, const void *b, size_t elem_size);

//================================================================================================================================
// BUFFER
//================================================================================================================================

/**
*   @brief Выделяет динамическую память для буфера buff размера buff_size
*
*   @param buff      [in] - буфер
*   @param buff_size [in] - размер буфера (в байтах)
*
*   @return true, если всё ОК
*   @return false в случае ошибки
*
*   @see buffer_ctor(buffer *const, const char *const)
*/
bool buffer_ctor(buffer *const buff, const size_t buff_size);

/**
*   @brief Выделяет динамическую память для содержимого файла <file_name>, заполняет поля буфера buff
*
*   @param buff      [in] - буфер
*   @param file_name [in] - имя файла
*
*   @return true если всё ОК
*   @return false в случае ошибки
*
*   @see buffer_ctor(buffer *const, const size_t)
*/
bool buffer_ctor(buffer *const buff, const char *const file_name);

/**
*   @brief Деструктор буфера
*/
void buffer_dtor(buffer *const buff);

#endif //ALGORITHM_H