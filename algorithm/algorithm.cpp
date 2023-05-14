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
// verify
//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static unsigned _buff_verify(const buffer *const buff)
{
$i
    unsigned err = BUFF_OK;

$   if (buff == nullptr) { buff_log_error(buff, err = (1 << BUFF_NULLPTR)); $o return err; }

$   err = buff_poison_verify(buff);
$   if (err != BUFF_OK) { buff_log_error(buff, err); $o return err; }

$   err = buff_fields_verify(buff);
$   if (err != BUFF_OK) { buff_log_error(buff, err); $o return err; }

$o  return BUFF_OK;
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

static void buff_log_error(const buffer *const buff, const unsigned err)
{
$i
    log_assert(err != BUFF_OK);

$   log_error("buffer verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(BUFF_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_error_message("%s", "\n", BUFF_STATUS_MESSAGES[i]);
    }

    log_message("\n");

$   buffer_static_dump(buff, true);

$   log_tab_error_message("====================", "\n");
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned buff_poison_verify(const buffer *const buff)
{
$i
    log_assert(buff != nullptr);

    unsigned err = BUFF_OK;

    if ($buff_beg  == BUFF_POISON.buff_beg ) err = err | (1 << BUFF_POISON_BEG );
    if ($buff_pos  == BUFF_POISON.buff_pos ) err = err | (1 << BUFF_POISON_POS );
    if ($buff_size == BUFF_POISON.buff_size) err = err | (1 << BUFF_POISON_SIZE);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned buff_fields_verify(const buffer *const buff)
{
$i
    log_assert(buff != nullptr);

    unsigned err = BUFF_OK;

    if ($buff_beg == nullptr) err = err | (1 << BUFF_BEG_NULLPTR);
    if ($buff_pos == nullptr) err = err | (1 << BUFF_POS_NULLPTR);

    if (err != BUFF_OK) { $o return err; }

    if ($buff_pos < $buff_beg             ) err = err | (1 << BUFF_POS_LESS_BEG);
    if ($buff_pos > $buff_beg + $buff_size) err = err | (1 << BUFF_POS_MORE_END);

$o  return err;
}

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

    buf_debug_verify(buff);

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

    buf_debug_verify(buff);

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
$   buf_verify(buff, (void) 0);

$   log_free($buff_beg);
    *buff = BUFF_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_free(void *const _buff)
{
$i
    if (_buff == nullptr) { $o return; }

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
$   buf_verify(buff           , false);
    log_verify(data != nullptr, false);

    size_t buff_size_left  = $buff_size - (size_t) ($buff_pos - $buff_beg);
$   size_t buff_size_write = size_t_min(buff_size_left, data_size);

$   memcpy($buff_pos, data, data_size);
    $buff_pos += buff_size_write;

$o  return buff_size_write == data_size;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_skip_spaces(buffer *const buff, size_t *const line_cnt /* = nullptr */)
{
    buf_verify(buff, false);

    if (line_cnt == nullptr) return buffer_skip_spaces_only(buff);

    size_t  line_add = 0;
    char   *buff_end = $buff_beg + $buff_size;
    while ($buff_pos != buff_end)
    {
        if (*$buff_pos == '\0') break;
        if (*$buff_pos == '\n') line_add++;
        if (!isspace(*$buff_pos)) break;

        $buff_pos++;
    }

    *line_cnt += line_add;

    return ($buff_pos != buff_end) && (*$buff_pos != '\0');
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline bool buffer_skip_spaces_only(buffer *const buff)
{
    buf_debug_verify(buff);

    char   *buff_end = $buff_beg + $buff_size;
    while ($buff_pos != buff_end)
    {
        if (*$buff_pos == '\0') return false;
        if (!isspace(*$buff_pos)) break;

        $buff_pos++;
    }

    return $buff_pos != buff_end;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void buffer_dump(const void *const _buff)
{
$i
    const buffer *const buff = (const buffer *) _buff;
$   buf_verify(buff, (void) 0);

$   buffer_static_dump(buff, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_static_dump(const buffer *const buff, const bool is_full)
{
$i
$   if (!buffer_header_dump(buff)) { $o return; }

$   bool are_invalid_public_fields =           buffer_public_fields_dump(buff);
$   bool are_invalid_static_fields = is_full ? buffer_static_fields_dump(buff) : false;

    bool are_invalid_fields = are_invalid_public_fields | are_invalid_static_fields;

$   buffer_content_dump(buff, are_invalid_fields);

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool __always_inline buffer_header_dump(const buffer *const buff)
{
$i
$   log_tab_service_message("buffer (addr: %p)\n"
                            "{", "\n",   buff);

$   if (buff == nullptr) { log_tab_service_message("}", "\n"); $o return false; }
    LOG_TAB++;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_public_fields_dump(const buffer *const buff)
{
$i
    log_assert(buff != nullptr);

    bool is_any_invalid = false;

    if ($buff_size == BUFF_POISON.buff_size) { $ poison_field_dump("size"); is_any_invalid = true; }
    else                                     { $ usual_field_dump ("size", "%lu", $buff_size);     }

    long rel_pos = $buff_pos - $buff_beg;

    if       (         rel_pos <          0) { $ error_field_dump("relative position", "%ld", rel_pos); is_any_invalid = true; }
    else if  ((size_t) rel_pos > $buff_size) { $ error_field_dump("relative position", "%ld", rel_pos); is_any_invalid = true; }
    else                                     { $ usual_field_dump("relative position", "%ld", rel_pos); }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_static_fields_dump(const buffer *const buff)
{
$i
    log_assert(buff != nullptr);

    bool is_any_invalid = false;

    if      ($buff_beg == BUFF_POISON.buff_beg) { $ poison_field_dump("buff_beg");                  is_any_invalid = true; }
    else if ($buff_beg == nullptr)              { $ error_field_dump ("buff_beg", "%p",   nullptr); is_any_invalid = true; }
    else                                        { $ usual_field_dump ("buff_beg", "%p", $buff_beg);                        }

    if      ($buff_pos == BUFF_POISON.buff_pos) { $ poison_field_dump("buff_pos");                  is_any_invalid = true; }
    else if ($buff_pos == nullptr)              { $ error_field_dump ("buff_pos", "%p",   nullptr); is_any_invalid = true; }
    else                                        { $ usual_field_dump ("buff_pos", "%p", $buff_pos);                        }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_content_dump(const buffer *const buff, const bool is_any_invalid)
{
$i
    log_assert(buff != nullptr);

$   log_tab_service_message("content\n"
                            "{", "\n");
    LOG_TAB++;

    if (is_any_invalid)
    {
$       log_tab_error_message("can't dump it because some of fields are invalid", "");
    }
    else { $ buffer_content_safety_dump(buff); }

    LOG_TAB--;
$   log_tab_service_message("\n}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_content_safety_dump(const buffer *const buff)
{
$i
    log_assert(buff      != nullptr);
    log_assert($buff_beg != nullptr);
    log_assert($buff_pos != nullptr);

    log_assert($buff_beg  != BUFF_POISON.buff_beg );
    log_assert($buff_pos  != BUFF_POISON.buff_pos );
    log_assert($buff_size != BUFF_POISON.buff_size);

$   log_tab_service_message("\"", "");

    const char      *buff_end = $buff_beg + $buff_size;
    for (const char *cur_char = $buff_beg; cur_char != buff_end; ++cur_char)
    {
        if      ( cur_char == $buff_pos) { $ log_message(HTML_COLOR_LIME_GREEN "|");   }

        if      (*cur_char ==      '\n') { $ log_service_message("\"\n\"", "");        }
        else if (*cur_char ==      '\0') { $ log_service_message("\""    , ""); break; }
        else                             { $ log_message     ("%c", *cur_char);        }

        if      ( cur_char == $buff_pos) { $ log_message("|" HTML_COLOR_CANCEL);       }
    }
$o
}
