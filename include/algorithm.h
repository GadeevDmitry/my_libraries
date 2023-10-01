/** @file */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../algorithm/src/algorithm_settings.h"

//================================================================================================================================

/**
*   @brief Структура для хранения массива символов.
*/
struct buffer
{
    char  *beg;    ///< указатель на начало буфера
    char  *pos;    ///< указатель на текущую позицию
    size_t size;   ///< размер буфера
};

//================================================================================================================================

extern const double DELTA; ///< погрешность сравнения вещественных чисел по умолчанию

//================================================================================================================================

/**
*   @brief Сравнивает два вещественных числа.
*
*   @param a          [in] - первое число
*   @param b          [in] - второе число
*   @param error_rate [in] - погрешность сравнения
*
*   @return 0, если a = b с учётом погрешности error_rate; < 0, если a < b; > 0, если a > b.
*/
int dblcmp(const double a, const double b, const double error_rate = DELTA);

/**
*   @brief Обменивает значения двух переменных.
*
*   @param a         [in, out] - указатель на первую переменную
*   @param b         [in, out] - указатель на вторую переменную
*   @param elem_size [in]      - размер (в байтах) переменной
*/
void my_swap(void *a, void *b, size_t elem_size);

/**
*   @brief Считывает space-символы до первого не space-символа.
*
*   @param stream [in] - поток
*
*   @return EOF, если после space-символов встретился конец файла, и 0 иначе.
*/
int skip_spaces(FILE *const stream);

/**
*   @brief Проверяет, есть ли символ в строке.
*
*   @param to_check [in] - символ
*   @param str      [in] - строка
*
*   @return true, если символ нашелся.
*/
bool is_char_in_str(const char to_check, const char *str);

//--------------------------------------------------------------------------------------------------------------------------------
// BUFFER
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор буфура.
*   @return 0, если буфер валидный.
*/
unsigned buffer_verify(const buffer *const buff);

/**
*   @brief Конструктор буфера.
*
*   @param buff      [out] - указатель на буфер
*   @param buff_size [in]  - размер буфера (в байтах)
*
*   @return true в случае успеха, false в случае ошибки.
*/
bool buffer_ctor(buffer *const buff, const size_t buff_size);

/**
*   @brief Конструктор буфера.
*
*   @param buff      [out] - указатель на буфер
*   @param file_name [in]  - имя файла, которым заполнить буфер
*
*   @return true в случае успеха, false в случае ошибки.
*/
bool buffer_ctor(buffer *const buff, const char *const file_name);

/**
*   @brief Создает буфер в динамической памяти.
*
*   @param buff_size [in] - размер буфера (в байтах)
*
*   @return указатель на созданный буфер или nullptr в случае ошибки.
*/
buffer *buffer_new(const size_t buff_size);

/**
*   @brief Создает буфер в динамической памяти.
*
*   @param file_name [in] - имя файла
*
*   @return указатель на созданный буфер, если все ОК, и nullptr в случае ошибки.
*/
buffer *buffer_new(const char *const file_name);

/**
*   @brief Деструктор буфера.
*
*   @param _buff [in] - указатель на буфер
*/
void buffer_dtor(void *const _buff);

/**
*   @brief Деструктор буфера в динамической памяти.
*
*   @see buffer_dtor(void* _buff)
*/
void buffer_delete(void *const _buff);

/**
*   @brief Записывает данные по адресу .buff_pos.
*   Если емкость буфера недостаточно велика, записывает, сколько поместится.
*
*   @param buff      [in, out] - указатель на буфер
*   @param data      [in]      - указатель на данные
*   @param data_size [in]      - кол-во байт в данных
*
*   @return true в случае успеха, false в случае ошибки или нехватки места.
*/
bool buffer_write(buffer *const buff, const void *data, const size_t data_size);

/**
*   @brief Возвращет true, если достигнут конец буфера или нулквой символ.
*/
bool buffer_is_end(buffer *const buff);

/**
*   @brief Пропускает символы-разделители в буфере.
*   Если line_cnt (указатель на счетчик строк) не равен nullptr, то функция будет вести подсчет пропущенных строк. Полученное
*   значение добавится к счетчику. Пробельные символы также являются символами-разделителями!
*
*   @param buff        [in, out] - указатель на буфер
*   @param split_chars [in]      - указатель на строку, состоящую из символов-разделителей
*   @param line_cnt    [out]     - указатель на счетчик строк (nullptr по умолчанию)
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
bool buffer_skip_split_chars(buffer *const buff, const char *split_chars, size_t *const line_cnt = nullptr);

/**
*   @brief Пропускает пробельные символы в буфере.
*   Если line_cnt (указатель на счетчик строк) не равен nullptr, то функция будет вести подсчет пропущенных строк. Полученное
*   значение добавится к счетчику.
*
*   @param buff     [in, out] - указатель на буфер
*   @param line_cnt [out]     - указатель на счетчик строк (nullptr по умолчанию)
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
bool buffer_skip_spaces(buffer *const buff, size_t *const line_cnt = nullptr);

/**
*   @brief Пропускает текущую строку в буфере.
*   Если line_cnt (указатель на счетчик строк) не равен nullptr, то функция будет вести подсчет пропущенных строк. Полученное
*   значение добавится к счетчику.
*
*   @param buff     [in, out] - указатель на буфер
*   @param line_cnt [out]     - указатель на счетчик строк (nullptr по умолчанию)
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
bool buffer_skip_line(buffer *const buff, size_t *const line_cnt = nullptr);

/**
*   @brief Переходит к следующей лексеме в буфере.
*   Для этого пропускается текущая лексема и символы-разделители за ней. Если line_cnt (указатель на счетчик строк) не равен nullptr,
*   то функция будет вести подсчет пропущенных строк. Полученное значение добавится к счетчику.
*   Пробельные символы также являются символами-разделителями!
*
*   @param buff        [in, out] - указатель на буфер
*   @param split_chars [in]      - указатель на строку, состоящую из символов-разделителей
*   @param line_cnt    [out]     - указатель на счетчик строк (nullptr по умолчанию)
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
bool buffer_next_token(buffer *const buff, const char *split_chars = nullptr, size_t *const line_cnt = nullptr);

/**
*   @brief Пропускает текущую лексему.
*   Для этого пропускаются символы, пока не встретится символ-разделитель. Если line_cnt (указатель на счетчик строк) не равен nullptr,
*   то функция будет вести подсчет пропущенных строк. Полученное значение добавится к счетчику.
*   Пробельные символы также являются символами-разделителями!
*
*   @param buff        [in, out] - указатель на буфер
*   @param split_chars [in]      - указатель на строку, состоящую из символов-разделителей
*   @param line_cnt    [out]     - указатель на счетчик строк (nullptr по умолчанию)
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
bool buffer_skip_token(buffer *const buff, const char *split_chars = nullptr);

/**
*   @brief Вычисляет размер текущей лексемы.
*   Для этого пробегаются символы, пока не встретится символ-разделитель.
*   Пробельные символы также являются символами-разделителями!
*
*   @param buff        [in, out] - указатель на буфер
*   @param split_chars [in]      - указатель на строку, состоящую из символов-разделителей
*
*   @return false в случае ошибки или если достигнут конец буфера или нулевой символ, иначе true.
*/
size_t buffer_get_token_size(const buffer *const buff, const char *split_chars = nullptr);

/**
*   @brief Дамп буфера в виде последовательности символов.
*
*   @param _buff [in] - буффер
*/
void buffer_dump(const void *const _buff);

/**
*   @brief Дамп буфера в виде последовательности hex-байтов.
*/
void buffer_hex_dump(const void *const _buff);

//================================================================================================================================

#ifndef BUFFER_NVERIFY
#define BUFFER_VERIFY(buf, ret_val) \
    if (buffer_verify(buf) != 0)    \
    {                               \
    $o  return ret_val;             \
    }
#else
#define BUFFER_VERIFY(buf, ret_val)
#endif

#ifndef BUFFER_NDEBUG
#define BUFFER_ASSERT(buf)                              \
    if (buffer_verify(buf) != 0)                        \
    {                                                   \
        fprintf(stderr, "BUFFER ASSERTION FAILED\n");   \
        abort();                                        \
    }
#else
#define BUFFER_ASSERT(buf)
#endif

#endif // ALGORITHM_H
