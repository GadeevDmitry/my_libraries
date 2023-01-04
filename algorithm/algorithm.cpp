#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "algorithm_static.h"
#include "algorithm.h"

#include "../logs/log.h"

//================================================================================================================================
// USEFUL FUNCTION
//================================================================================================================================

double dblcmp(const double a, const double b, const double error_rate /*= DELTA*/)
{
    if (fabs(a - b) < error_rate) return 0;
    return a - b;
}

void my_swap(void *a, void *b, size_t elem_size)
{
    assert(a != nullptr);
    assert(b != nullptr);

    for (; sizeof(long long) <= elem_size; elem_size -= sizeof(long long))
    {
        long long temp  = *(long long *)a;
        *(long long *)a = *(long long *)b;
        *(long long *)b = temp;

        a = (long long *)a + 1;
        b = (long long *)b + 1;
    }

    for (; sizeof(int) <= elem_size; elem_size -= sizeof(int))
    {
        int temp  = *(int *)a;
        *(int *)a = *(int *)b;
        *(int *)b = temp;

        a = (int *)a + 1;
        b = (int *)b + 1;
    }

    for (; sizeof(char) <= elem_size; elem_size -= sizeof(char))
    {
        unsigned char temp  = *(unsigned char *)a;
        *(unsigned char *)a = *(unsigned char *)b;
        *(unsigned char *)b = temp;

        a = (unsigned char *)a + 1;
        b = (unsigned char *)b + 1;
    }
}

//================================================================================================================================
// BUFFER
//================================================================================================================================

bool buffer_ctor(buffer *const buff, const size_t buff_size)
{
    assert(buff != nullptr);

    buff->buff_beg = (char *) log_calloc(buff_size, sizeof(char));
    if (buff->buff_beg == nullptr)
    {
        log_error("log_calloc(size, sizeof(char)) returns nullptr\n");
        return false;
    }

    buff->buff_pos  = buff->buff_beg;
    buff->buff_size = buff_size;

    return true;
}

bool buffer_ctor(buffer *const buff, const char *const file_name)
{
    assert(file_name != nullptr);
    assert(buff      != nullptr);

    if (!get_file_size(file_name, &buff->buff_size)) return false;
    buff->buff_size += 1;                                                   //for null character at the end

    buff->buff_beg = (char *) log_calloc(buff->buff_size, sizeof(char));
    if (buff->buff_beg == nullptr)
    {
        log_error("log_calloc(buff->buff_size, sizeof(char)) returns nullptr\n");
        return false;
    }

    FILE *const stream = fopen(file_name, "r");
    if (stream == nullptr)
    {
        log_error("fopen(\"%s\", \"r\") returns nullptr\n", file_name);
        return false;
    }

    fread(buff->buff_beg, sizeof(char), buff->buff_size, stream);
    buff->buff_beg[buff->buff_size - 1] = '\0';
    buff->buff_pos = buff->buff_beg;

    return true;
}

static bool get_file_size(const char *file_name, size_t *const file_size)
{
    assert(file_name != nullptr);
    assert(file_size != nullptr);

    struct stat file_info = {};
    if (stat(file_name, &file_info) == -1)
    {
        log_error("system call \"stat\" of file \"%s\" returns -1\n", file_name);
        return false;
    }

    *file_size = (size_t) file_info.st_size;
    return true;
}

void buffer_dtor(buffer *const buff)
{
    if (buff == nullptr) return;

    log_free(buff->buff_beg);

    buff->buff_beg  = nullptr;
    buff->buff_pos  = nullptr;
    buff->buff_size = 0;
}