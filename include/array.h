/** @file */
#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

//================================================================================================================================

/**
*   @brief Структура данных массив.
*/
struct array
{
    void     *data; ///< указатель на начало массива
    size_t    size; ///< размер массива
    size_t el_size; ///< размер элемента массива

    void (*el_dtor) (      void *const el); ///< указатель на dtor элемента массива
    void (*el_dump) (const void *const el); ///< указатель на dump элемента массива
};

//================================================================================================================================

/**
*   @brief Верификатор массива.
*   @return 0, если массив валидный.
*/
unsigned array_verify(const array *const arr);

/**
*   @brief Конструктор массива.
*
*   @param arr     [out] - указатель на массив
*   @param size    [in]  - размер массива
*   @param el_size [in]  - размер элемента массива
*   @param el_dtor [in]  - указатель на dtor элемента массива
*   @param el_dump [in]  - указатель на dump элемента массива
*
*   @return true, если удалось создать массив, false - иначе.ы
*/
bool array_ctor(array *const arr, const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) = nullptr,
                                                                           void (*el_dump) (const void *const el) = nullptr);

/**
*   @brief Создает массив в динамической памяти.
*
*   @param arr     [out] - указатель на массив
*   @param size    [in]  - размер массива
*   @param el_size [in]  - размер элемента массива
*   @param el_dtor [in]  - указатель на dtor элемента массива
*   @param el_dump [in]  - указатель на dump элемента массива
*
*   @return указатель на созданный массив или nullptr в случае ошибки.
*/
array *array_new(const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) = nullptr,
                                                          void (*el_dump) (const void *const el) = nullptr);

/**
*   @brief Деструктор массива.
*
*   @param _arr [in] - указатель на массив
*/
void array_dtor(void *const _arr);

/**
*   @brief Деструктор массива в динамической памяти.
*
*   @see array_dtor(void* _arr)
*/
void array_delete(void *const _arr);

/**
*   @brief Инициализирует все ячейки массива заданным значением.
*
*   @param arr       [in, out] - указатель на массив
*   @param init_elem [in]      - указатель на значение для инициализации
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool array_init(array *const arr, const void *const init_elem);

/**
*   @brief Устанавливает ячейку массива заданным значением.
*
*   @param arr   [in, out] - указатель на массив
*   @param index [in]      - индекс ячейки
*   @param elem  [in]      - указатель на значение для установки
*
*   @return true в случае успеха, false в случае ошибки.
*/
bool array_set(array *const arr, const size_t index, const void *const elem);

/**
*   @brief Возвращает указатель на ячейку массива.
*
*   @param arr   [in] - указатель на массив
*   @param index [in] - индекс ячейки
*/
void *array_get(const array *const arr, const size_t index);

/**
*   @brief Возвращает указатель на начало массива - его первый элемент.
*/
void *array_begin(const array *const arr);

/**
*   @brief Возвращает указатель на конец массива - элемент, следующий за последним.
*/
void *array_end(const array *const arr);

/**
*   @brief Дамп массива.
*
*   @param _arr [in] - указатель на массив
*/
void array_dump(const void *const _arr);

//================================================================================================================================

#if defined(NVERIFY)
#define ARRAY_NVERIFY
#endif

#if defined(NDEBUG)
#define ARRAY_NDEBUG
#endif

//--------------------------------------------------------------------------------------------------------------------------------

#ifndef ARRAY_NVERIFY
#define ARRAY_VERIFY(arr, ret_val)  \
    if (array_verify(arr) != 0)     \
    {                               \
    $o  return ret_val;             \
    }
#else
#define ARRAY_VERIFY(arr, ret_val)
#endif

#ifndef ARRAY_NDEBUG
#define ARRAY_ASSERT(arr)                           \
    if (array_verify(arr) != 0)                     \
    {                                               \
        fprintf(stderr, "ARRAY ASSERTION FAILED\n");\
        abort();                                    \
    }
#else
#define ARRAY_ASSERT(arr)
#endif

#endif //ARRAY_H
