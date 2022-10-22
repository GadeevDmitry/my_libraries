#ifndef VAR_DECLARATION
#define VAR_DECLARATION

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

struct var_declaration
{
    const char *name_file, *name_func, *name_var;
    uint64_t    line;    
};

/*___________________________FUNCTION_DECLARATION___________________________*/

static void var_ctor (var_declaration *const var, const char *name_file, const char *name_func, const char *name_var, const uint64_t line);
static void var_dtor (var_declaration *const var, const var_declaration *const poison);
static void var_dump (var_declaration *const var, const var_declaration *const poison);

/*__________________________________________________________________________*/

#endif //VAR_DECLARATION