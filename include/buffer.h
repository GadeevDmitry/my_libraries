/** @file */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

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
*   @brief Возвращает указатель на конец буфера - элемент, следующий за последним.
*
*   @param buff - указатель на буфер
*/
char *buffer_end(const buffer *const buff);

/**
*   @brief Возвращает указатель на последний элемент буфера.
*
*   @param buff - указатель на буфер
*/
char *buffer_back(const buffer *const buff);

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

#endif //BUFFER_H
