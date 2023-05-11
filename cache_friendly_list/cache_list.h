/** @file */
#ifndef CACHE_LIST_H
#define CACHE_LIST_H

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Вершина cache-friendly листа.
*/
struct cache_list_node
{
    const void *data;   ///< указатель на элемент листа
    size_t      prev;   ///< индекс предыдущей вершины листа
    size_t      next;   ///< индекс следущей вершины листа
};

/**
*   @brief Структура данных cache-friendly лист (кэш-лист).
*/
struct cache_list
{
    cache_list_node *fictional;             ///< указатель на массив вершин листа

    size_t el_free;                         ///< индекс свободного элемента листа
    size_t    size;                         ///< количество элементов в листе
    size_t capacity;                        ///< емкость массива вершин листа

    void (*el_dump) (const void *const);    ///< указатель на dump элемента листа
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

/**
*   @brief cache_list_ctor.
*
*   @param lst     [out] - указатель на кэш-лист
*   @param el_dump [in]  - указатель на dump элемента листа
*/
bool cache_list_ctor(cache_list *const lst, void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает кэш лист в динамической памяти.
*
*   @param el_dump [in] - указатель на dump элемента листа
*
*   @return указатель на созданный лист или nullptr в случае ошибки
*/
cache_list *cache_list_new(void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief cache_list_dtor.
*
*   @param _lst [in] - указатель на кэш-лист
*/
void cache_list_dtor(void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Вызывает cache_list_dtor(void* _lst) и освобождает динамическую память, на которую должен указывать кэш-лист.
*
*   @see cache_list_dtor(void* _lst)
*/
void cache_list_free(void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в кэш-лист.
*
*   @param lst   [in, out] - указатель на кэш-лист
*   @param data  [in]      - указатель на элемент
*   @param index [in]      - порядковый номер, на который поставить элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool cache_list_insert(cache_list *const lst, const void *const data, const size_t pos);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в начало кэш-листа.
*
*   @param lst  [in, out] - указатель на кэш-лист
*   @param data [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool cache_list_push_front(cache_list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в конец кэш-листа.
*
*   @param lst  [in, out] - указатель на кэш-лист
*   @param data [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool cache_list_push_back(cache_list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из кэш-листа.
*
*   @param lst  [in, out] - указатель на кэш-лист
*   @param pos  [in]      - порядковый номер удаляемого элемента
*
*   @return указатель на содержимое удаляемой вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_erase(cache_list *const lst, const size_t pos);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из начала кэш-листа.
*
*   @param lst  [in, out] - указатель на кеш-лист
*
*   @return указатель на содержимое удаляемой вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_pop_front(cache_list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из конца кэш-листа.
*
*   @param lst  [in, out] - указатель на кэш-лист
*
*   @return указатель на содержимое удаляемой вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_pop_back(cache_list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое элемента кэш-листа.
*
*   @param lst  [in] - указатель на кэш-лист
*   @param pos  [in] - порядковый номер элемента
*
*   @return указатель на содержимое вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_get(const cache_list *const lst, const size_t pos);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое первого элемента кэш-листа.
*
*   @param lst  [in]  - указатель на кэш-лист
*
*   @return указатель на содержимое вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_front(const cache_list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое последнего элемента кэш-листа.
*
*   @param lst  [in]  - указатель на лист
*
*   @return указатель на содержимое вершины, если все ОК, nullptr в случае ошибки
*/
void *cache_list_back(const cache_list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Ищет элемент в кеш-листе.
*
*   @param lst      [in] - указатель на кеш-лист
*   @param target   [in] - указатель на элемент, который нужно найти
*   @param elem_cmp [in] - указатель на функцию сравнения элементов, которая возвращает 0, если элементы равны
*
*   @return указатель на первое вхождение найденного элемента или nullptr, если его в листе нет.
*/
void *cache_list_find(const cache_list *const lst, const void *const target, int (*elem_cmp)(const void *elem_1, const void *elem_2));

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Ищет элемент в кеш-листе.
*   Работает корректно, если верно предположение: все элементы кеш-листа лежат в памяти без промежутков, начиная сразу после фиктивной вершины.
*   Функция обходит первые .size слотов, итерируясь как в массиве, возвращает указатель на первое (в порядке массива) вхождение найденного элемента.
*
*   @param lst      [in] - указатель на кеш-лист
*   @param target   [in] - указатель на элемент, который нужно найти
*   @param elem_cmp [in] - указатель на функцию сравнения элементов, которая возвращает 0, если элементы равны
*
*   @return указатель на первое (в порядке массива) вхождение найденного элемента или nullptr, если его в листе нет.
*/
void *cache_list_find_through(const cache_list *const lst, const void *const target, int (*elem_cmp)(const void *elem_1, const void *elem_2));

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп кэш-листа.
*
*   @param lst [in] - указатель на кэш-лист
*/
void cache_list_dump(const void *const _lst);

#endif // CACHE_LIST_H
