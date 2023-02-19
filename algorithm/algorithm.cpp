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

//--------------------------------------------------------------------------------------------------------------------------------
// dblcmp
//--------------------------------------------------------------------------------------------------------------------------------

static int _dblcmp(const double a, const double b, const double error_rate /* = DELTA */)
{
    if (fabs(a - b) < error_rate) return 0;

    if (a < b) return -1;
    return 1;
}

int _dblcmp(const char *const cur_file,
            const char *const cur_func,
            const int         cur_line,

            const double a, const double b, const double error_rate /* = DELTA */)
{
    trace_push(cur_file, cur_func, cur_line);
    int ret = _dblcmp(a, b, error_rate);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// my_swap
//--------------------------------------------------------------------------------------------------------------------------------

static void _my_swap(void *a, void *b, size_t elem_size)
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

void _my_swap(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              void *a, void *b, size_t elem_size)
{
    trace_push(cur_file, cur_func, cur_line);
    _my_swap(a, b, elem_size);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// is_byte_equal
//--------------------------------------------------------------------------------------------------------------------------------

static bool _is_byte_equal(const void *a, const void *b, size_t elem_size)
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

bool _is_byte_equal(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    const void *a, const void *b, size_t elem_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _is_byte_equal(a, b, elem_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// get_word
//--------------------------------------------------------------------------------------------------------------------------------

static int _get_word(char *const str, const size_t max_size, FILE *const stream)
{
    log_verify(str    != nullptr, EOF);
    log_verify(stream != nullptr, EOF);

    if (skip_spaces(stream) == EOF) return EOF;

    unsigned int cur_char = 0;
    int                 i = 0;

    for (; i < max_size - 1; ++i)
    {
        cur_char = getc(stream);

        if (cur_char == EOF) break;
        if (isspace(cur_char))
        {
            ungetc(cur_char, stream);
            break;
        }
        str[i] = cur_char;
    }
    str[i] = '\0';

    if (i == max_size - 1)
    {
        cur_char = getc(stream);
        if (cur_char != EOF && !isspace(cur_char)) { ungetc(cur_char, stream); return 1; }
        else if (               isspace(cur_char)) { ungetc(cur_char, stream);           }
    }

    return 0;
}

int _get_word(const char *const cur_file,
              const char *const cur_func,
              const int         cur_line,

              char *const str, const size_t max_size, FILE *const stream)
{
    trace_push(cur_file, cur_func, cur_line);
    int ret = _get_word(str, max_size, stream);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// skip_spaces
//--------------------------------------------------------------------------------------------------------------------------------

static int _skip_spaces(FILE *const stream)
{
    log_verify(stream != nullptr, ;);

    unsigned int cur_char = 0;
    while ((cur_char = getc(stream)) != EOF)
    {
        if (!isspace(cur_char))
        {
            ungetc(cur_char, stream);
            break;
        }
    }

    return cur_char == EOF ? EOF : 0;
}

int _skip_spaces(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 FILE *const stream)
{
    trace_push(cur_file, cur_func, cur_line);
    int ret = _skip_spaces(stream);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

static bool _buffer_ctor(buffer *const buff, const size_t buff_size)
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

bool _buffer_ctor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  buffer *const buff, const size_t buff_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _buffer_ctor(buff, buff_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _buffer_ctor_file(buffer *const buff, const char *const file_name)
{
    log_verify(file_name != nullptr, false);
    log_verify(buff      != nullptr, false);

    if (!get_file_size(file_name, &buff->buff_size)) return false;
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

bool _buffer_ctor_file(const char *const cur_file,
                       const char *const cur_func,
                       const int         cur_line,

                       buffer *const buff, const char *const file_name)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _buffer_ctor_file(buff, file_name);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _get_file_size(const char *file_name, size_t *const file_size)
{
    log_assert(file_name != nullptr);
    log_assert(file_size != nullptr);

    struct stat file_info = {};
    if (stat(file_name, &file_info) == -1)
    {
        log_error("system call \"stat\" of file \"%s\" returns -1\n", file_name);
        return false;
    }

    *file_size = (size_t) file_info.st_size;
    return true;
}

static bool _get_file_size(const char *const cur_file,
                           const char *const cur_func,
                           const int         cur_line,

                           const char *const file_name, size_t *const file_size)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _get_file_size(file_name, file_size);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_dtor(void *const _buff)
{
    buffer *const buff = (buffer *) _buff;
    if (buff == nullptr) return;

    log_free(buff->buff_beg);

    buff->buff_beg  = nullptr;
    buff->buff_pos  = nullptr;
    buff->buff_size = 0;
}

void _buffer_dtor(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  void *const _buff)
{
    trace_push(cur_file, cur_func, cur_line);
    _buffer_dtor(_buff);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_dump(const void *const _buff)
{
    const buffer *const buff = (const buffer *) _buff;

    log_tab_message("buffer (address: %p)\n"
                    "{\n",          buff);
    LOG_TAB++;

    if (buff == nullptr) { LOG_TAB--; log_tab_message("}\n"); return; }

    if (buff->buff_beg == nullptr) error_field_dump("buff_beg ", "%p" , buff->buff_beg);
    else                           usual_field_dump("buff_beg ", "%p" , buff->buff_beg);

    if (buff->buff_pos == nullptr) error_field_dump("buff_pos ", "%p" , buff->buff_pos);
    else                           usual_field_dump("buff_pos ", "%p" , buff->buff_pos);

    usual_field_dump                               ("buff_size", "%lu", buff->buff_size);

    if (buff->buff_beg != nullptr && buff->buff_pos != nullptr)
    {
        log_tab_message(HTML_COLOR_MEDIUM_BLUE "\n"
                        "buff_pos - buff_beg = %d"
                        HTML_COLOR_CANCEL "\n", buff->buff_pos - buff->buff_beg);
    }
    if (buff->buff_beg != nullptr) buffer_content_dump(buff);

    LOG_TAB--;
    log_tab_message("\n}\n");
}

void _buffer_dump(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const void *const _buff)
{
    trace_push(cur_file, cur_func, cur_line);
    _buffer_dump(_buff);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _buffer_content_dump(const buffer *const buff)
{
    log_assert(buff           != nullptr);
    log_assert(buff->buff_beg != nullptr);

    log_tab_message("buffer_content:\n"
                    HTML_COLOR_MEDIUM_BLUE "\"" HTML_COLOR_CANCEL);

    for (const char *cur_char = buff->buff_beg; (size_t) (cur_char - buff->buff_beg) < buff->buff_size; ++cur_char)
    {
        if      ( cur_char == buff->buff_pos) { log_message(HTML_COLOR_LIME_GREEN "|"); }

        if      (*cur_char ==           '\n') { log_message(HTML_COLOR_MEDIUM_BLUE "\"\n\"" HTML_COLOR_CANCEL); }
        else if (*cur_char ==           '\0') { log_message(HTML_COLOR_MEDIUM_BLUE "\""     HTML_COLOR_CANCEL); break; }
        else                                  { log_message("%c", *cur_char); }

        if      ( cur_char == buff->buff_pos) { log_message("|" HTML_COLOR_CANCEL); }
    }
}

static void _buffer_content_dump(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const buffer *const buff)
{
    trace_push(cur_file, cur_func, cur_line);
    _buffer_content_dump(buff);
    trace_pop();
}
