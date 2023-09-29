/** @file */
#ifndef VECTOR_STATIC_H
#define VECTOR_STATIC_H

#include "vector.h"

//================================================================================================================================

#define VECTOR_GET(vec, ind) (char *) vec->data + vec->el_size * ind

#endif // VECTOR_STATIC_H
