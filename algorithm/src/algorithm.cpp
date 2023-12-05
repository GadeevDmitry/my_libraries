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

//--------------------------------------------------------------------------------------------------------------------------------

int skip_spaces(FILE *const stream)
{
$i
    LOG_VERIFY(stream != nullptr, EOF);

    int cur_char = 0;
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

//--------------------------------------------------------------------------------------------------------------------------------

bool is_char_in_str(const char to_check, const char *str)
{

    if (str == nullptr) return false;

    for (; *str != '\0'; ++str)
        if (*str == to_check) return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

unsigned buffer_verify(const buffer *const buff)
{
$i
    unsigned err = BUFF_OK;

$   if (buff == nullptr)
    {
        err = (1 << BUFF_NULLPTR);
$       buffer_log_error(buff, err);
$o      return err;
    }

$   err = buffer_fields_verify(buff);

$   buffer_log_error(buff, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_log_error(const buffer *const buff, const unsigned err)
{
$i
    if (err == BUFF_OK) { $o return; }

$   LOG_ERROR("buffer verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(BUFF_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i))
            LOG_TAB_ERROR_MESSAGE("%s", "\n", BUFF_STATUS_MESSAGES[i]);
    }

$   LOG_MESSAGE("\n");
$   buffer_static_dump(buff, true);
$   LOG_TAB_ERROR_MESSAGE(BOLD_LOG_SEP, "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned buffer_fields_verify(const buffer *const buff)
{
$i
    LOG_ASSERT(buff != nullptr);

    unsigned err = BUFF_OK;

    if (buff->beg  == BUFF_POISON.beg ) err |= (1 << BUFF_POISON_BEG );
    if (buff->pos  == BUFF_POISON.pos ) err |= (1 << BUFF_POISON_POS );
    if (buff->size == BUFF_POISON.size) err |= (1 << BUFF_POISON_SIZE);

    if (buff->beg == nullptr) err |= (1 << BUFF_BEG_NULLPTR);
    if (buff->pos == nullptr) err |= (1 << BUFF_POS_NULLPTR);

    if (err != BUFF_OK) { $o return err; }

    if (buff->pos < buff->beg             ) err |= (1 << BUFF_POS_LESS_BEG);
    if (buff->pos > buff->beg + buff->size) err |= (1 << BUFF_POS_MORE_END);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_ctor(buffer *const buff, const size_t buff_size)
{
$i
    LOG_VERIFY(buff != nullptr, false);

$   buff->beg = (char *) LOG_CALLOC(buff_size, sizeof(char));
    if (buff->beg == nullptr)
    {
$       LOG_ERROR("log_calloc(buff_size = %lu, sizeof(char) = %lu) returns nullptr\n",
                              buff_size,       sizeof(char));
$o      return false;
    }

    buff->pos  = buff->beg;
    buff->size = buff_size;

    BUFFER_ASSERT(buff);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_ctor(buffer *const buff, const char *const file_name)
{
$i
    LOG_VERIFY(buff      != nullptr, false);
    LOG_VERIFY(file_name != nullptr, false);

$   if (!get_file_size(file_name, &buff->size)) { $o return false; }
    buff->size += 1;    //for null character at the end

$   buff->beg = (char *) LOG_CALLOC(buff->size, sizeof(char));
    if (buff->beg == nullptr)
    {
$       LOG_ERROR("log_calloc(.size = %lu, sizeof(char) = %lu) returns nullptr\n",
                         buff->size,       sizeof(char));
$o      return false;
    }

    FILE *const stream = fopen(file_name, "r");
    if (stream == nullptr)
    {
$       LOG_ERROR("fopen(\"%s\", \"r\") returns nullptr\n", file_name);
$o      return false;
    }

    fread(buff->beg, sizeof(char), buff->size - 1, stream);
    buff->beg[buff->size - 1] = '\0';
    buff->pos = buff->beg;

    fclose(stream);

    BUFFER_ASSERT(buff);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

buffer *buffer_new(const size_t buff_size)
{
$i
$   buffer *buff = (buffer *) LOG_CALLOC(1, sizeof(buffer));
    if (buff == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(buffer) = %lu) returns nullptr\n", sizeof(buffer));
$o      return nullptr;
    }

$   if (!buffer_ctor(buff, buff_size))
    {
        LOG_FREE(buff);
$o      return nullptr;
    }

    BUFFER_ASSERT(buff);
$o  return buff;
}

//--------------------------------------------------------------------------------------------------------------------------------

buffer *buffer_new(const char *const file_name)
{
$i
$   buffer *buff = (buffer *) LOG_CALLOC(1, sizeof(buffer));
    if (buff == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(buffer) = %lu) returns nullptr\n", sizeof(buffer));
$o      return nullptr;
    }

$   if (!buffer_ctor(buff, file_name))
    {
        LOG_FREE(buff);
        return nullptr;
    }

    BUFFER_ASSERT(buff);
$o  return buff;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool get_file_size(const char *file_name, size_t *const file_size)
{
$i
    LOG_ASSERT(file_name != nullptr);
    LOG_ASSERT(file_size != nullptr);

    struct stat file_info = {};
    if (stat(file_name, &file_info) == -1)
    {
$       LOG_ERROR("system call \"stat\" of file \"%s\" returns -1\n", file_name);
$o      return false;
    }

    *file_size = (size_t) file_info.st_size;
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_dtor(void *const _buff)
{
$i
    if (_buff == nullptr) { $o return; }

    buffer *const buff = (buffer *) _buff;
$   BUFFER_VERIFY(buff, (void) 0);

$   LOG_FREE(buff->beg);
    *buff = BUFF_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_delete(void *const _buff)
{
$i
$   buffer_dtor(_buff);
$   LOG_FREE   (_buff);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

char *buffer_end(const buffer *const buff)
{
    BUFFER_VERIFY(buff, nullptr);
    return buff->beg + buff->size;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_write(buffer *const buff, const void *data, const size_t data_size)
{
$i
$   BUFFER_VERIFY(buff           , false);
    LOG_VERIFY   (data != nullptr, false);

    size_t buff_size_left  = buff->size - (size_t) (buff->pos - buff->beg);
$   size_t buff_size_write = std::min(buff_size_left, data_size);

$   memcpy(buff->pos, data, buff_size_write);
    buff->pos += buff_size_write;

$o  return buff_size_write == data_size;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_is_end(buffer *const buff)
{
$i
$   BUFFER_VERIFY(buff, false);

    char *buff_end = buff->beg + buff->size;
$o  return (buff->pos == buff_end) || (*buff->pos == '\0');
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_skip_split_chars(buffer *const buff, const char *split_chars, size_t *const line_cnt /* = nullptr */)
{
$i
$   BUFFER_VERIFY(buff, false);

    if (split_chars == nullptr) { $o return buffer_skip_spaces(buff, line_cnt); }

    size_t line_add = 0;
    char  *buff_end = buff->beg + buff->size;
$   for (; buff->pos != buff_end; ++buff->pos)
    {
        if (*buff->pos == '\n') { line_add++; continue; }
        if (*buff->pos == '\0')   break;

        if (isspace       (*buff->pos))              continue;
        if (is_char_in_str(*buff->pos, split_chars)) continue;

        break;
    }

    if (line_cnt != nullptr) *line_cnt += line_add;

$o  return (buff->pos != buff_end) && (*buff->pos != '\0');
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_skip_spaces(buffer *const buff, size_t *const line_cnt /* = nullptr */)
{
$i
$   BUFFER_VERIFY(buff, false);

    size_t line_add = 0;
    char  *buff_end = buff->beg + buff->size;
$   for (; buff->pos != buff_end; ++buff->pos)
    {
        if (*buff->pos == '\n') { line_add++; continue; }
        if (*buff->pos == '\0')   break;
        if (!isspace(*buff->pos)) break;
    }

    if (line_cnt != nullptr) *line_cnt += line_add;

$o  return (buff->pos != buff_end) && (*buff->pos != '\0');
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_skip_line(buffer *const buff, size_t *const line_cnt /* = nullptr */)
{
$i
$   BUFFER_VERIFY(buff, false);

    char *buff_end = buff->beg + buff->size;
$   for (; buff->pos != buff_end; ++buff->pos)
    {
        if (*buff->pos == '\0') { $o return false; }
        if (*buff->pos == '\n') { buff->pos++; break; }
    }

    if (line_cnt != nullptr) *line_cnt += 1;

$o  return buff->pos != buff_end;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_next_token(buffer *const buff, const char *split_chars /* = nullptr */, size_t *const line_cnt /* = nullptr */)
{
$i
$   BUFFER_VERIFY(buff, false);

$   if (!buffer_skip_token(buff, split_chars)) { $o return false; }
$   bool ret = buffer_skip_split_chars(buff, split_chars, line_cnt);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_skip_token(buffer *const buff, const char *split_chars /* = nullptr */)
{
$i
$   BUFFER_ASSERT(buff);

    char *buff_end = buff->beg + buff->size;
$   for (; buff->pos != buff_end; ++buff->pos)
    {
        if (*buff->pos == '\0') { $o return false; }

        if (is_char_in_str(*buff->pos, split_chars)) { $o return true; }
        if (isspace       (*buff->pos))              { $o return true; }
    }

$o  return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t buffer_get_token_size(const buffer *const buff, const char *split_chars /* = nullptr */)
{
$i
$   BUFFER_VERIFY(buff, 0);

    size_t size_token = 0;

    char *buff_end = buff->beg + buff->size;
    char *buff_pos = buff->pos;
$   for (; buff_pos != buff_end; ++buff_pos)
    {
        if (*buff_pos == '\0') break;
        if (isspace       (*buff_pos))              break;
        if (is_char_in_str(*buff_pos, split_chars)) break;

        size_token++;
    }

$o  return size_token;
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_dump(const void *const _buff)
{
$i
    const buffer *buff = (const buffer *) _buff;
$   BUFFER_VERIFY(buff, (void) 0);
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
    bool are_invalid_fields        = are_invalid_public_fields | are_invalid_static_fields;

$   buffer_content_dump(buff, are_invalid_fields);

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool __always_inline buffer_header_dump(const buffer *const buff)
{
$i
$   LOG_TAB_SERVICE_MESSAGE("buffer (address: %p)\n"
                            "{", "\n", buff);

$   if (buff == nullptr)
    {
        LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o      return false;
    }

    LOG_TAB++;
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_public_fields_dump(const buffer *const buff)
{
$i
    LOG_ASSERT(buff != nullptr);

    bool is_any_invalid = false;
    long rel_pos        = buff->pos - buff->beg;

    if (buff->size == BUFF_POISON.size)      { $ POISON_FIELD_DUMP("size             "); is_any_invalid = true; }
    else                                     { $ USUAL_FIELD_DUMP ("size             ", "%lu", buff->size); }

    if       (         rel_pos <          0) { $ ERROR_FIELD_DUMP ("relative position", "%ld", rel_pos); is_any_invalid = true; }
    else if  ((size_t) rel_pos > buff->size) { $ ERROR_FIELD_DUMP ("relative position", "%ld", rel_pos); is_any_invalid = true; }
    else                                     { $ USUAL_FIELD_DUMP ("relative position", "%ld", rel_pos); }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool buffer_static_fields_dump(const buffer *const buff)
{
$i
    LOG_ASSERT(buff != nullptr);

    bool is_any_invalid = false;

    if      (buff->beg == BUFF_POISON.beg) { $ POISON_FIELD_DUMP("buff_beg");                  is_any_invalid = true; }
    else if (buff->beg == nullptr)         { $ ERROR_FIELD_DUMP ("buff_beg", "%p",   nullptr); is_any_invalid = true; }
    else                                   { $ USUAL_FIELD_DUMP ("buff_beg", "%p", buff->beg); }

    if      (buff->pos == BUFF_POISON.pos) { $ POISON_FIELD_DUMP("buff_pos");                  is_any_invalid = true; }
    else if (buff->pos == nullptr)         { $ ERROR_FIELD_DUMP ("buff_pos", "%p",   nullptr); is_any_invalid = true; }
    else                                   { $ USUAL_FIELD_DUMP ("buff_pos", "%p", buff->pos); }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_content_dump(const buffer *const buff, const bool is_any_invalid)
{
$i
    LOG_ASSERT(buff != nullptr);

$   LOG_TAB_SERVICE_MESSAGE("content\n" "{", "\n");
    LOG_TAB++;

    if (is_any_invalid) { $ LOG_TAB_ERROR_MESSAGE("can't dump it because some of fields are invalid", ""); }
    else                { $ buffer_content_safety_dump(buff); }

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("\n}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_content_safety_dump(const buffer *const buff)
{
$i
    LOG_ASSERT(buff      != nullptr);
    LOG_ASSERT(buff->beg != nullptr);
    LOG_ASSERT(buff->pos != nullptr);

    LOG_ASSERT(buff->beg  != BUFF_POISON.beg );
    LOG_ASSERT(buff->pos  != BUFF_POISON.pos );
    LOG_ASSERT(buff->size != BUFF_POISON.size);

$   LOG_TAB_SERVICE_MESSAGE("\"", "");

    const char *buff_end = buff->beg + buff->size;
    for (const char *cur_char = buff->beg; cur_char != buff_end; ++cur_char)
    {
        if      ( cur_char == buff->pos) { $ log_message(HTML_COLOR_LIME_GREEN "|");   }

        if      (*cur_char ==      '\n') { $ LOG_SERVICE_MESSAGE("\"\n\"", "");        }
        else if (*cur_char ==      '\0') { $ LOG_SERVICE_MESSAGE("\""    , ""); break; }
        else                             { $ log_message        ("%c", *cur_char);     }

        if      ( cur_char == buff->pos) { $ log_message("|" HTML_COLOR_CANCEL);       }
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void buffer_hex_dump(const void *const _buff)
{
    const buffer *buff = (const buffer *) _buff;
    BUFFER_VERIFY(buff, (void) 0);

    buffer_header_dump     (buff);
    buffer_hex_content_dump(buff);

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void buffer_hex_content_dump(const buffer *const buff)
{
    BUFFER_ASSERT(buff);

    USUAL_FIELD_DUMP("beg ", "%p" , buff->beg );
    USUAL_FIELD_DUMP("pos ", "%p" , buff->pos );
    USUAL_FIELD_DUMP("size", "%lu", buff->size);

    LOG_MESSAGE("\n");

    const unsigned char *pos = (unsigned char *) buff->beg;
    const unsigned char *end = (unsigned char *) buff->beg + buff->size;

    for (size_t y = 1; pos != end           ; ++y) { LOG_TAB_MESSAGE(" ");
    for (size_t x = 1; pos != end && x <= 16; ++x)
        {
            unsigned char high = (*pos >> 4);
            unsigned char low  = (*pos % 16);
            pos++;

            LOG_MESSAGE("%x%x ", high, low);

            if (x % 4 == 0) LOG_MESSAGE(" ");
        }

        LOG_MESSAGE("\n");
        if (y % 4 == 0) LOG_MESSAGE("\n");
    }
}
