#ifndef LOG_DEF_H
#define LOG_DEF_H

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

#define log_message(    fmt, ...)         log_message    (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_tab_message(fmt, ...)         log_tab_message(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_header(     fmt, ...)         log_header     (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_param_place(file, func, line) log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__, file, func, line  )

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

#define log_calloc(number, size) log_calloc (__FILE__, __PRETTY_FUNCTION__, __LINE__, number, size)
#define log_realloc(  ptr, size) log_realloc(__FILE__, __PRETTY_FUNCTION__, __LINE__,    ptr, size)
#define log_free(     ptr      ) log_free   (__FILE__, __PRETTY_FUNCTION__, __LINE__,    ptr      )

#endif //LOG_DEF_H