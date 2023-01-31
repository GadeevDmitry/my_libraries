#ifndef ALG_DEF_H
#define ALG_DEF_H

//--------------------------------------------------------------------------------------------------------------------------------
// USEFUL FUNCTION
//--------------------------------------------------------------------------------------------------------------------------------

#define dblcmp(       a, b,       ...) dblcmp       (__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b, ##__VA_ARGS__)
#define my_swap(      a, b, elem_size) my_swap      (__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b,     elem_size)
#define is_byte_equal(a, b, elem_size) is_byte_equal(__FILE__, __PRETTY_FUNCTION__, __LINE__, a, b,     elem_size)

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

#define buffer_ctor(     buff, buff_size) buffer_ctor     (__FILE__, __PRETTY_FUNCTION__, __LINE__, buff, buff_size)
#define buffer_ctor_file(buff, file_name) buffer_ctor_file(__FILE__, __PRETTY_FUNCTION__, __LINE__, buff, file_name)
#define buffer_dtor(     buff           ) buffer_dtor     (__FILE__, __PRETTY_FUNCTION__, __LINE__, buff           )

#endif //ALG_DEF_H