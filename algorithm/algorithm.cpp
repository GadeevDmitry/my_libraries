#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "algorithm_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// USEFUL FUNCTION
//--------------------------------------------------------------------------------------------------------------------------------

static int dblcmp(const double a, const double b, const double error_rate /* = DELTA */)
{
    if (fabs(a - b) < error_rate) return 0;

    if (a < b) return -1;
    return 1;
}

int dblcmp(const char *const cur_file,
           const char *const cur_func,
           const int         cur_line,

           const double a, const double b, const double error_rate /* = DELTA */)
{
    trace_push(cur_file, cur_func, cur_line);
    int ret = dblcmp(a, b, error_rate);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void my_swap(void *a, void *b, size_t elem_size)
{
    log_verify(a != nullptr, ;);
    log_verify(b != nullptr, ;);

    for (; elem_size > 0; elem_size--)
    {
        char temp   = *(char *) a;
        *(char *) a = *(char *) b;
        *(char *) b =        temp;

        a = (char *) a + 1;
        b = (char *) b + 1;
    }
}

void my_swap(const char *const cur_file,
             const char *const cur_func,
             const int         cur_line,

             void *a, void *b, size_t elem_size)
{
    trace_push(cur_file, cur_func, cur_line);
    my_swap(a, b, elem_size);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool is_byte_equal(const void *a, const void *b, size_t elem_size)
{
    log_verify(a != nullptr, false);
    log_verify(b != nullptr, false);

    for (; elem_size > 0; elem_size--)
    {
        char a_val = *(const char *) a;
        char b_val = *(const char *) b;

        if (a_val != b_val) return false;

        a = (const char *) a + 1;
        b = (const char *) b + 1;
    }

    return true;
}

bool is_byte_equal(const char *const cur_file,
                   const char *const cur_func,
                   const int         cur_line,

                   const void *a, const void *b, size_t elem_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = is_byte_equal(a, b, elem_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

static bool buffer_ctor(buffer *const buff, const size_t buff_size)
{
    log_verify(buff != nullptr, false);

    buff->buff_beg = (char *) log_calloc(buff_size, sizeof(char));
    if (buff->buff_beg == nullptr)
    {
        log_error("log_calloc(buff_size = %lu, sizeof(char) = %lu) returns nullptr\n",
                              buff_size,       sizeof(char));
        return false;
    }

    buff->buff_pos  = buff->buff_beg;
    buff->buff_size = buff_size;

    return true;
}

bool buffer_ctor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 buffer *const buff, const size_t buff_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = buffer_ctor(buff, buff_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool buffer_ctor_file(buffer *const buff, const char *const file_name)
{
    log_verify(file_name != nullptr, false);
    log_verify(buff      != nullptr, false);

    if (!get_file_size(__FILE__, __PRETTY_FUNCTION__, __LINE__, file_name, &buff->buff_size)) return false;
    buff->buff_size += 1; //for null character at the end

    buff->buff_beg = (char *) log_calloc(buff->buff_size, sizeof(char));
    if (buff->buff_beg == nullptr)
    {
        log_error("log_calloc(buff->buff_size = %lu, sizeof(char) = %lu) returns nullptr\n",
                              buff->buff_size,       sizeof(char));
        return false;
    }

    FILE *const stream = fopen(file_name, "r");
    if (stream == nullptr)
    {
        log_error("fopen(\"%s\", \"r\") returns nullptr\n", file_name);
        return false;
    }

    fread(buff->buff_beg, sizeof(char), buff->buff_size - 1, stream);
    buff->buff_beg[buff->buff_size - 1] = '\0';
    buff->buff_pos = buff->buff_beg;

    fclose(stream);

    return true;
}

bool buffer_ctor_file(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      buffer *const buff, const char *const file_name)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = buffer_ctor_file(buff, file_name);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

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

static bool get_file_size(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          const char *const file_name, size_t *const file_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = get_file_size(file_name, file_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_dtor(buffer *const buff)
{
    if (buff == nullptr) return;

    log_free(buff->buff_beg);

    buff->buff_beg  = nullptr;
    buff->buff_pos  = nullptr;
    buff->buff_size = 0;
}

void buffer_dtor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 buffer *const buff)
{
    trace_push(cur_file, cur_func, cur_line);
    buffer_dtor(buff);
    trace_pop();
}