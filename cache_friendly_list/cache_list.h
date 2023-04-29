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
    void  *data;    ///< указатель на начало элемента листа
    size_t prev;    ///< индекс предыдущей вершины листа
    size_t next;    ///< индекс следущей вершины листа
};

/**
*   @brief Структура данных cache-friendly лист.
*/
struct cache_list
{
    cache_list_node *fictional;             ///< указатель на массив вершин листа

    size_t el_size;                         ///< размер (в байтах) элемента листа
    size_t el_free;                         ///< индекс свободного элемента листа
    size_t    size;                         ///< количество элементов в листе
    size_t capacity;                        ///< емкость массива вершин листа

    void (*el_dtor) (      void *const);    ///< указатель на dtor элемента листа
    void (*el_dump) (const void *const);    ///< указатель на dump элемента листа
};

#endif // CACHE_LIST_H