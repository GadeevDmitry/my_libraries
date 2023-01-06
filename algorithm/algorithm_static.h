/** @file */
#ifndef ALGORITHM_STATIC_H
#define ALGORITHM_STATIC_H

#include "algorithm.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// FUNCTION DECLARATION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/**
*   @brief Находит размер файла <file_name>
*
*   @param file_name [in]  - имя файла
*   @param file_size [out] - размер файла
*
*   @return true, если всё ОК
*   @return false в случае ошибки
*/
static bool get_file_size(const char *file_name, size_t *const file_size);

#endif //ALGORITHM_STATIC_H