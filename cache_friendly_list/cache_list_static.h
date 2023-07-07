/** @file */
#ifndef CACHE_LIST_STATIC_H
#define CACHE_LIST_STATIC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cache_list_settings.h"
#include "cache_list.h"

//================================================================================================================================
// DSL
//================================================================================================================================

typedef cache_list_node list_node;
typedef cache_list      list;

//--------------------------------------------------------------------------------------------------------------------------------
// list_node
//--------------------------------------------------------------------------------------------------------------------------------

#define $is_busy    (lst_node->is_busy)
#define $prev       (lst_node->prev)
#define $next       (lst_node->next)

//--------------------------------------------------------------------------------------------------------------------------------
// list
//--------------------------------------------------------------------------------------------------------------------------------

#define $fictional  (lst->fictional)
#define $data       (lst->data)

#define $size       (lst->size)
#define $capacity   (lst->capacity)

#define $el_free    (lst->el_free)
#define $el_size    (lst->el_size)

#define $el_dtor    (lst->el_dtor)
#define $el_dump    (lst->el_dump)

//================================================================================================================================
// GLOBAL
//================================================================================================================================

/**
*   @brief Типы ошибок в листе
*/
enum LST_STATUS
{
    LST_OK                      ,   ///< OK
    LST_NULLPTR                 ,   ///< lst = nullptr

    LST_POISON_FICTIONAL        ,   ///< .fictional = LST_POISON.service
    LST_POISON_DATA             ,   ///< .data      = LST_POISON.data
    LST_POISON_SIZE             ,   ///< .size      = LST_POISON.size
    LST_POISON_CAPACITY         ,   ///< .capacity  = LST_POISON.capacity
    LST_POISON_EL_FREE          ,   ///< .el_free   = LST_POISON.free
    LST_POISON_EL_SIZE          ,   ///< .el_size   = LST_POISON.size
    LST_POISON_EL_DTOR          ,   ///< .el_dtor   = LST_POISON.el_dtor
    LST_POISON_EL_DUMP          ,   ///< .el_dump   = LST_POISON.el_dump

    LST_NULLPTR_FICTIONAL       ,   ///< .fictional = nullptr
    LST_NULLPTR_DATA            ,   ///< .data      = nullptr
    LST_INVALID_SIZE_CAPACITY   ,   ///< .size      >= .capacity
    LST_INVALID_EL_FREE         ,   ///< .el_free   >  .capacity

    LST_INVALID_CYCLE           ,   ///< нарушен цикл
};

/**
*   @brief Сообщения для каждого типа ошибок
*
*   @see enum LST_STATUS
*/
static const char *LST_STATUS_MESSAGES[] =
{
    "cache_list is OK"                                          ,
    "cache_list is nullptr"                                     ,

    "cache_list.fictional"" is poison"                          ,
    "cache_list.data"     " is poison"                          ,
    "cache_list.size"     " is poison"                          ,
    "cache_list.capacity" " is poison"                          ,
    "cache_list.el_free"  " is poison"                          ,
    "cache_list.el_size"  " is poison"                          ,
    "cache_list.el_dtor"  " is poison"                          ,
    "cache_list.el_dump"  " is poison"                          ,

    "cache_list.fictional is nullptr"                           ,
    "cache_list.data"   " is nullptr"                           ,
    "cache_list.size >= cache_list.capacity"                    ,
    "cache_list.el_free"" is invalid"                           ,

    "cycle of list_node is invalid"                             ,
};

/**
*   @brief POISON-значения полей листа
*/
static const list LST_POISON =
{
    (list_node *) 0xABADBABE,   // fictional
    (void      *) 0xDEADBEEF,   // data

    -1UL                    ,   // size
    -1UL                    ,   // capacity

    -1UL                    ,   // el_free
    -1UL                    ,   // el_size

    (void (*)(      void *)) 0xABADF00D, // el_dtor
    (void (*)(const void *)) 0xBADCAB1E, // el_dump
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщения об ошибках в кэш-листе по битовой маске ошибок. Полный дамп кэш-листа.
*/
static void list_log_error(const list *const lst, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет на наличие POISON-полей в кэш-листе.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_poison_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор полей кэш-листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_fields_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор свободных элементов кэш-листа и цикла из них.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_free_cycle_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор занятых элементов кэш-листа и цикла из них.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_busy_cycle_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор вершины кэш-листа.
*
*   @param lst      [in] - указатель на лист, к которому относится вершина
*   @param lst_node [in] - указатель на вершину для верификации
*   @param is_busy  [in] - true, если вершина должна быть "занятой", false - иначе
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned _list_node_verify(const list *const lst, const list_node *const lst_node, const bool is_busy);

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конструктор полей .fictional и .data кэш-листа.
*   Выделяет динамическую память для вершин кэш-листа, инициализирует фиктивную вершину листа и цикл свободных элементов.
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool list_data_ctor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Инициализирует цикл свободных элементов.
*/
static void list_free_cycle_ctor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Инициализирует поля свободной вкршины кэш-листа.
*
*   @param lst [in, out] - указатель на кэш-лист
*
*   @param ind_cur  [in] - индекс свободной вершины в массиве вершин
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*/
static void list_free_node_init(list *const lst, const size_t ind_cur ,
                                                 const size_t ind_prev,
                                                 const size_t ind_next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Переводит вершину из цикла занятых элементов в цикл свободных.
*
*   @param lst         [in, out] - указатель на кэш-лист
*   @param ind_cur     [in]      - индекс вершины в массиве
*   @param erased_data [out]     - указатель, по которому скопировать содержимое удаляемой вершины (nullptr, если копировать не надо)
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool list_free_node_new(list *const lst, const size_t ind_cur, void *const erased_data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief конструктор свободной вершины кэш-листа.
*   В отличии от list_free_node_init обновляет поля смежных в цикле вершин, чтобы связность не нарушилась.
*
*   @param lst [in, out] - указатель на кэш-лист
*
*   @param ind_cur  [in] - индекс свободной вершины в массиве вершин
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*
*   @see list_free_node_init(list* lst, const size_t ind_cur , const size_t ind_prev, const size_t ind_next)
*/
static void list_free_node_ctor(list *const lst, const size_t ind_cur,
                                                 const size_t ind_next,
                                                 const size_t ind_prev);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Переводит вершину из цикла свободных элементов в цикл занятых.
*
*   @param lst [in, out] - указатель на кэш-лист
*   @param data    [out] - указатель, откуда скопировать содержимое кэш-листа
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool list_busy_node_new(list *const lst, const void *const data, const size_t ind_prev,
                                                                        const size_t ind_next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief конструктор занятой вершины кэш-листа.
*
*   @param lst [in, out] - указатель на кэш-лист
*   @param data     [in] - указатель, откуда скопировать содержимое кэш-листа
*   @param ind_cur  [in] - индекс занятой вершины в массиве вершин
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*/
static void list_busy_node_ctor(list *const lst, const void *const data, const size_t ind_cur,
                                                                         const size_t ind_prev,
                                                                         const size_t ind_next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Реаллоцирует кэш-лист.
*/
static bool list_resize(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает индекс вершины в массиве по ее порядковому номеру в кэш-листе.
*/
static size_t list_get_node_index(const list *const lst, const size_t pos);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп кеш-листа.
*   В "полном" режиме (is_full = true), помимо пользовательских данных, дампит и служебные. Используется для дампа невалидного кеш-листа.
*
*   @param lst     [in] - указатель на кеш-лист
*   @param is_full [in] - true, если нужен дамп со служебной информацией, false иначе
*/
static void list_static_dump(const list *const lst, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Шапка" дампа кэш-листа.
*
*   @return true, если lst != nullptr
*/
static __always_inline bool list_header_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп "public" полей кэш-листа.
*
*   @return true, если хоть одно поле невалидно
*/
static bool list_public_fields_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп "static" полей кэш-листа.
*
*   @return true, если хоть одно поле невалидно
*/
static bool list_static_fields_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого массива кэш-листа.
*
*   @param lst            [in] - указатель на кеш-лист для дампа
*   @param is_full        [in] - true, если нужен дамп "static" полей
*   @param is_any_invalid [in] - true, если хоть одно поле невалидно
*/
static void list_data_dump(const list *const lst, const bool is_full,
                                                  const bool is_any_invalid);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Debug" дамп содержимого кэш-листа.
*   Дамп в порядке индексации в массиве, содержит свободные элементы.
*/
static void list_data_debug_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Debug" дамп вершины кэш-листа.
*   Дамп полей вершины, вызов .el_dump, если вершина занята.
*/
static void list_node_debug_dump(const list *const lst, const list_node *const lst_node, const void *const el_data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Pretty" дамп массива кэш-листа.
*   Дамп в порядке кэш-листа, не содержит свободных элементов.
*/
static void list_data_pretty_dump(const list *const lst);

#endif //CACHE_LIST_STATIC_H
