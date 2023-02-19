#ifndef ALG_DEF_H
#define ALG_DEF_H

//--------------------------------------------------------------------------------------------------------------------------------
// USEFUL FUNCTION
//--------------------------------------------------------------------------------------------------------------------------------

#define dblcmp(       a, b,       ...) _dblcmp       (__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b, ##__VA_ARGS__)
#define my_swap(      a, b, elem_size) _my_swap      (__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b,     elem_size)
#define is_byte_equal(a, b, elem_size) _is_byte_equal(__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b,     elem_size)

//--------------------------------------------------------------------------------------------------------------------------------

#define get_word(str, max_size, stream) _get_word   (__FILE__, __PRETTY_FUNCTION__, __LINE__, str, max_size, stream)
#define skip_spaces(            stream) _skip_spaces(__FILE__, __PRETTY_FUNCTION__, __LINE__,                stream)

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

#define buffer_ctor(     buff, buff_size) _buffer_ctor     (__FILE__, __PRETTY_FUNCTION__, __LINE__, buff, buff_size)
#define buffer_ctor_file(buff, file_name) _buffer_ctor_file(__FILE__, __PRETTY_FUNCTION__, __LINE__, buff, file_name)
#define buffer_dtor(     buff           ) _buffer_dtor     (__FILE__, __PRETTY_FUNCTION__, __LINE__, buff           )
#define buffer_dump(     buff           ) _buffer_dump     (__FILE__, __PRETTY_FUNCTION__, __LINE__, buff           )

#endif //ALG_DEF_H