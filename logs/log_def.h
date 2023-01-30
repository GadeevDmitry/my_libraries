#ifndef LOG_DEF_H
#define LOG_DEF_H

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------

#define log_message(    fmt, ...) log_message    (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_tab_message(fmt, ...) log_tab_message(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_header(     fmt, ...) log_header     (__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_param_place(file, func, line) log_param_place(__FILE__, __PRETTY_FUNCTION__, __LINE__, file, func, line)

//--------------------------------------------------------------------------------------------------------------------------------
// LOG_MEMORY
//--------------------------------------------------------------------------------------------------------------------------------

#define log_calloc( number, size) log_calloc (number, size, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define log_realloc(ptr   , size) log_realloc(ptr   , size, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define log_free(   ptr)          log_free   (ptr   ,       __FILE__, __PRETTY_FUNCTION__, __LINE__)

#endif //LOG_DEF_H