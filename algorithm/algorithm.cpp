#include "algorithm_static.h"

//================================================================================================================================
// USEFUL FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// dblcmp
//--------------------------------------------------------------------------------------------------------------------------------

int dblcmp(const double a, const double b, const double error_rate /* = DELTA */)
{
$i
    if (fabs(a - b) < error_rate) { $o return 0; }

    if (a < b) { $o return -1; }
    /*else */    $o return  1;
}

//--------------------------------------------------------------------------------------------------------------------------------
// my_swap
//--------------------------------------------------------------------------------------------------------------------------------

void my_swap(void *a, void *b, size_t elem_size)
{
$i
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
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// is_byte_equal
//--------------------------------------------------------------------------------------------------------------------------------

bool is_byte_equal(const void *a, const void *b, size_t elem_size)
{
$i
    log_verify(a != nullptr, false);
    log_verify(b != nullptr, false);

    for (; elem_size > 0; elem_size--)
    {
        char a_val = *(const char *) a;
        char b_val = *(const char *) b;

        if (a_val != b_val) { $o return false; }

        a = (const char *) a + 1;
        b = (const char *) b + 1;
    }

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// skip_spaces
//--------------------------------------------------------------------------------------------------------------------------------

int skip_spaces(FILE *const stream)
{
$i
    log_verify(stream != nullptr, EOF);

    int     cur_char = 0;
    while ((cur_char = getc(stream)) != EOF)
    {
        if (!isspace(cur_char))
        {
            ungetc(cur_char, stream);
            break;
        }
    }

$o  return cur_char == EOF ? EOF : 0;
}

//================================================================================================================================
// BUFFER
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_ctor(buffer *const buff, const size_t buff_size)
{
$i
    log_verify(buff != nullptr, false);

$   $buff_beg = (char *) log_calloc(buff_size, sizeof(char));
    if ($buff_beg == nullptr)
    {
$       log_error("log_calloc(buff_size = %lu, sizeof(char) = %lu) returns nullptr\n",
                              buff_size,       sizeof(char));
$o      return false;
    }

    $buff_pos  = $buff_beg;
    $buff_size = buff_size;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_ctor(buffer *const buff, const char *const file_name)
{
$i
    log_verify(file_name != nullptr, false);
    log_verify(buff      != nullptr, false);

$   if (!get_file_size(file_name, &$buff_size)) { $o return false; }
    $buff_size += 1;    //for null character at the end

$   $buff_beg = (char *) log_calloc($buff_size, sizeof(char));
    if ($buff_beg == nullptr)
    {
$       log_error("log_calloc(.buff_size = %lu, sizeof(char) = %lu) returns nullptr\n",
                              $buff_size,       sizeof(char));
$o      return false;
    }

    FILE *const stream = fopen(file_name, "r");
    if (stream == nullptr)
    {
$       log_error("fopen(\"%s\", \"r\") returns nullptr\n", file_name);
$o      return false;
    }

    fread($buff_beg, sizeof(char), $buff_size - 1, stream);
    $buff_beg[$buff_size - 1] = '\0';
    $buff_pos = $buff_beg;

    fclose(stream);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

buffer *buffer_new(const size_t buff_size)
{
$i
$   buffer *buff_new = (buffer *) log_calloc(1, sizeof(buffer));
    if (buff_new == nullptr)
    {
$       log_error("log_calloc(1, sizeof(buffer) = %lu) returns nullptr\n", sizeof(buffer));
$o      return nullptr;
    }

$   if (!buffer_ctor(buff_new, buff_size)) { log_free(buff_new); $o return nullptr; }
$o  return buff_new;
}

//--------------------------------------------------------------------------------------------------------------------------------

buffer *buffer_new(const char *const file_name)
{
$i
$   buffer *buff_new = (buffer *) log_calloc(1, sizeof(buffer));
    if (buff_new == nullptr)
    {
$       log_error("log_calloc(1, sizeof(buffer) = %lu) returns nullptr\n", sizeof(buffer));
$o      return nullptr;
    }

$   if (!buffer_ctor(buff_new, file_name)) { log_free(buff_new); return nullptr; }
$o  return buff_new;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool get_file_size(const char *file_name, size_t *const file_size)
{
$i
    log_assert(file_name != nullptr);
    log_assert(file_size != nullptr);

    struct stat file_info = {};
    if (stat(file_name, &file_info) == -1)
    {
$       log_error("system call \"stat\" of file \"%s\" returns -1\n", file_name);
$o      return false;
    }

    *file_size = (size_t) file_info.st_size;
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_dtor(void *const _buff)
{
$i
    buffer *const buff = (buffer *) _buff;
    if (buff == nullptr) { $o return; }

$   log_free($buff_beg);

    $buff_beg  = nullptr;
    $buff_pos  = nullptr;
    $buff_size =       0;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_free(void *const _buff)
{
$i
$   buffer_dtor(_buff);
$   log_free   (_buff);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// query
//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_write(buffer *const buff, const void *data, const size_t data_size)
{
$i
    log_verify(buff != nullptr, false);
    log_verify(data != nullptr, false);

    size_t buff_size_left  = $buff_size - (size_t) ($buff_pos - $buff_beg);
$   size_t buff_size_write = size_t_min(buff_size_left, data_size);

$   memcpy($buff_pos, data, data_size);
    $buff_pos += buff_size_write;

$o  return buff_size_write == data_size;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void buffer_dump(const void *const _buff)
{
$i
    const buffer *const buff = (const buffer *) _buff;

$   log_tab_message("buffer (address: %p)\n"
                    "{\n",          buff);
    LOG_TAB++;

$   if (buff == nullptr) { LOG_TAB--; log_tab_message("}\n"); $o return; }

    if ($buff_beg == nullptr) { $ error_field_dump("buff_beg ", "%p" , $buff_beg); }
    else                      { $ usual_field_dump("buff_beg ", "%p" , $buff_beg); }

    if ($buff_pos == nullptr) { $ error_field_dump("buff_pos ", "%p" , $buff_pos); }
    else                      { $ usual_field_dump("buff_pos ", "%p" , $buff_pos); }

$   usual_field_dump                              ("buff_size", "%lu", $buff_size);

    if ($buff_beg != nullptr && $buff_pos != nullptr)
    {
$       log_tab_service_message("\n"
                                "buff_pos - buff_beg = %d", "\n", $buff_pos - $buff_beg);
    }
$   if ($buff_beg != nullptr) buffer_content_dump(buff);

    LOG_TAB--;
$   log_tab_message("\n}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_content_dump(const buffer *const buff)
{
$i
    log_assert(buff      != nullptr);
    log_assert($buff_beg != nullptr);

$   log_tab_message("buffer_content:\n"
                    HTML_COLOR_MEDIUM_BLUE "\"" HTML_COLOR_CANCEL);

    for (const char *cur_char = $buff_beg; (size_t) (cur_char - $buff_beg) < $buff_size; ++cur_char)
    {
        if      ( cur_char == $buff_pos) { $ log_message(HTML_COLOR_LIME_GREEN "|"); }

        if      (*cur_char ==      '\n') { $ log_message(HTML_COLOR_MEDIUM_BLUE "\"\n\"" HTML_COLOR_CANCEL); }
        else if (*cur_char ==      '\0') { $ log_message(HTML_COLOR_MEDIUM_BLUE "\""     HTML_COLOR_CANCEL); break; }
        else                             { $ log_message("%c", *cur_char); }

        if      ( cur_char == $buff_pos) { $ log_message("|" HTML_COLOR_CANCEL); }
    }
$o
}
