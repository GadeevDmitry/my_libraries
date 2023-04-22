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
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
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

/**
*   @brief Dump содержимого буффера.
*
*   @param buff [in] - буффер
*/
static void buffer_content_dump(const buffer *const buff);

//================================================================================================================================
// MACRO DEFINITIONS
//================================================================================================================================

#define $buff_beg   (buff->buff_beg)
#define $buff_pos   (buff->buff_pos)
#define $buff_size  (buff->buff_size)

#endif //ALGORITHM_STATIC_H