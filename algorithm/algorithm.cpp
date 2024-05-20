#include "algorithm_static.h"

//================================================================================================================================

int dblcmp(const double a, const double b, const double error_rate /* = DELTA */)
{
$i
    LOG_VERIFY(isfinite(a)         , -1);
    LOG_VERIFY(isfinite(b)         , -1);
    LOG_VERIFY(isfinite(error_rate), -1);

    if (fabs(a - b) < error_rate) { $o return 0; }

    if (a < b) { $o return -1; }
$o  return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

void my_swap(void *a, void *b, size_t elem_size)
{
$i
    LOG_VERIFY(a != nullptr, (void) 0);
    LOG_VERIFY(b != nullptr, (void) 0);

    for (; elem_size > 0; elem_size--)
    {
        char temp   = *(char *) a;
        *(char *) a = *(char *) b;
        *(char *) b =        temp;

        a = (char *) a + 1;
        b = (char *) b + 1;
    }
$o
}
