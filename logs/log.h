/** @file */
#ifndef LOG_H
#define LOG_H

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// GLOBAL
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

extern size_t LOG_TAB;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// MACRO DEFENITIONS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define HTML_COLOR_GOLD         "<font color=Gold>"
#define HTML_COLOR_DARK_RED     "<font color=DarkRed>"
#define HTML_COLOR_DARK_ORANGE  "<font color=DarkOrange>"
#define HTML_COLOR_LIME_GREEN   "<font color=LimeGreen>"
#define HTML_COLOR_MEDIUM_BLUE  "<font color=MediumBlue>"
#define HTML_COLOR_OLIVE        "<font color=Olive>"
#define HTML_COLOR_BLACK        "<font color=Black>"
#define HTML_COLOR_CANCEL       "</font>"

/**
*   @brief выводит в лог файл имя файла, имя функции, номер строки в точке вызова
*/
#define log_place()                                                                 \
        log_tab_message("\n"                                                        \
                        "    FILE: %s\n"                                            \
                        "FUNCTION: %s\n"                                            \
                        "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

/**
*   @brief assert с сообщением в лог файл
*/
#define log_assert(condition)                                   \
            if  (!(condition))                                  \
            {                                                   \
			    log_tab_message(HTML_COLOR_DARK_RED             \
                                "ASSERT FAILED: %s\n"           \
					            "         FILE: %s\n"           \
					            "     FUNCTION: %s\n"           \
					            "         LINE: %d\n"           \
                                HTML_COLOR_CANCEL    ,          \
                                                                \
				                #condition           ,          \
						        __FILE__		     ,          \
						        __PRETTY_FUNCTION__	 ,          \
						        __LINE__		      );        \
			abort();                                            \
            }

/**
*   @brief Выводит сообение об ошике в заданной параметрами точке в лог файл
*
*   @see log_error(fmt, ...)
*   @see log_message(const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
#define log_param_error(file, func, line, fmt, ...)             \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_RED                     \
                        "\nERROR:\n");                          \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_tab_message("    FILE: %s\n"                        \
                        "FUNCTION: %s\n"                        \
                        "    LINE: %d"      ,                   \
                    file                    ,                   \
                    func                    ,                   \
                    line                    );                  \
        log_message    (HTML_COLOR_CANCEL "\n\n");              \
        }

/**
*   @brief Выводит сообщение об ошибке в точке вызова в лог файл
*
*   @see log_param_error(file, func, line, fmt, ...)
*   @see log_message(const char *fmt, ...)
*   @see log_tab_message(const char *fmt, ...)
*   @see log_warning(const char *fmt, ...)
*/
#define log_error(fmt, ...) log_param_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__);

/**
*   @brief Выводит warning в лог файл
*
*   @see log_message(const char *fmt, ...)
*   @see log_tab_message(const char *fmt, ...)
*/
#define log_warning(fmt, ...)                                   \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_ORANGE "\nWARNING:\n"); \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_message    (HTML_COLOR_CANCEL);                     \
        }

/**
*   @brief Выводит warning c параметрами в лог
*
*   @see log_warning(fmt, ...)
*   @see log_inline_warning(fmt, ...)
*/
#define log_param_warning(file, func, line, fmt, ...)           \
        {                                                       \
        log_tab_message(HTML_COLOR_DARK_ORANGE                  \
                        "\nWARNING:\n");                        \
        log_tab_message(fmt, ## __VA_ARGS__);                   \
        log_tab_message("    FILE: %s\n"                        \
                        "FUNCTION: %s\n"                        \
                        "    LINE: %d\n",                       \
                    file,                                       \
                    func,                                       \
                    line);                                      \
        log_message(HTML_COLOR_CANCEL);                         \
        }

/**
*   @brief warning в точке вызова
*
*   @see log_warning(fmt, ...)
*   @see log_param_warning(file, func, line, fmt, ...)
*/
#define log_inline_warning(fmt, ...) log_param_warning(__FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// FUNCTION DECLARATION
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//================================================================================================================================
// LOG_OUTPUT
//================================================================================================================================

/**
*   @brief Выводит сообщение в лог файл, используя vfprintf()
*   Не использует log_tab() перед выводом первой строки
*
*   @see log_tab_message(const char *fmt, ...)
*/
void log_message       (const char *fmt, ...);

/**
*   @brief Выводит сообщение в лог файл, используя vfprintf()
*   Использует log_tab() перед выводом первой строки
*
*   @see log_message(const char *fmt, ...)
*/
void log_tab_message   (const char *fmt, ...);

/**
*   @brief HTML-заголовок
*/
void log_header        (const char *fmt, ...);

/**
*   @brief Выводит в лог файл имя файла, имя функции, номер строки
*
*   @param file [in] - имя файла
*   @param func [in] - имя функции
*   @param line [in] - номер строки
*/
void log_param_place   (const char *file,
                        const char *func,
                        const int   line);


//================================================================================================================================
// LOG_MEMORY
//================================================================================================================================

/**
*   @brief Запрашивает динамическую память, используя calloc(). Увеличивает DYNAMIC_MEMORY, если запрос успешен
*
*   @see DYNAMIC_MEMORY
*   @see log_realloc(void *, size_t)
*   @see log_free(void *)
*/
void *log_calloc  (size_t number, size_t size);

/**
*   @brief Меняет размер блока динамической памяти, используя realloc(). Меняет DYNAMIC_MEMORY
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_free(void *)
*/
void *log_realloc (void *ptr, size_t size);

/**
*   @brief Освобождает блок динамической памяти, используя free(). Уменяшает DYNAMIC_MEMORY, если блок был не пуст
*
*   @see DYNAMIC_MEMORY
*   @see log_calloc(size_t, size_t)
*   @see log_realloc(void *, size_t)
*/
void  log_free    (void *ptr);

#endif //LOG_H