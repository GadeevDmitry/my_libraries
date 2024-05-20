/** @file */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdlib.h>

//================================================================================================================================

extern const double DELTA; ///< погрешность сравнения вещественных чисел по умолчанию

//================================================================================================================================

/**
*   @brief Сравнивает два вещественных числа.
*
*   @param a          [in] - первое число
*   @param b          [in] - второе число
*   @param error_rate [in] - погрешность сравнения
*
*   @return 0, если a = b с учётом погрешности error_rate; < 0, если a < b; > 0, если a > b.
*/
int dblcmp(const double a, const double b, const double error_rate = DELTA);

/**
*   @brief Обменивает значения двух переменных.
*
*   @param a         [in, out] - указатель на первую переменную
*   @param b         [in, out] - указатель на вторую переменную
*   @param elem_size [in]      - размер (в байтах) переменной
*/
void my_swap(void *a, void *b, size_t elem_size);

#endif // ALGORITHM_H
