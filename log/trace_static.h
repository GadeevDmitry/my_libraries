#ifndef TRACE_STATIC_H
#define TRACE_STATIC_H

#include "log.h"
#include "stack.h"
#include "trace.h"

#include <stdlib.h>

//================================================================================================================================

bool   trace_ctor();
void   trace_dtor();
void   trace_dump();
size_t trace_get_size();

#endif // TRACE_STATIC_H
