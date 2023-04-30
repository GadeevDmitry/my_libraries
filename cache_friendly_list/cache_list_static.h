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

#define $data       (lst_node->data)
#define $prev       (lst_node->prev)
#define $next       (lst_node->next)

//--------------------------------------------------------------------------------------------------------------------------------
// list
//--------------------------------------------------------------------------------------------------------------------------------

#define $fictional  (lst->fictional)

#define $el_size    (lst->el_size)
#define $el_free    (lst->el_free)
#define $size       (lst->size)
#define $capacity   (lst->capacity)

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
    LST_OK                          ,   ///< OK
    LST_NULLPTR                     ,   ///< lst = nullptr

    LST_POISON_FICTIONAL            ,   ///< .fictional        = LST_POISON.fictional
    LST_POISON_EL_SIZE              ,   ///< .el_size          = LST_POISON.el_size
    LST_POISON_EL_FREE              ,   ///< .el_free          = LST_POISON.free
    LST_POISON_SIZE                 ,   ///< .size             = LST_POISON.size
    LST_POISON_CAPACITY             ,   ///< .capacity         = LST_POISON.capacity
    LST_POISON_EL_DTOR              ,   ///< .el_dtor          = LST_POISON.el_dtor
    LST_POISON_EL_DUMP              ,   ///< .el_dump          = LST_POISON.el_dump

    LST_NULLPTR_FICTIONAL           ,   ///< .fictional        = nullptr
    LST_FICTIONAL_DATA_NOT_NULLPTR  ,   ///< .fictional->data != nullptr
    LST_INVALID_SIZE_CAPACITY       ,   ///< .size > .capacity
    LST_INVALID_EL_FREE             ,   ///< .el_free > .size

    LST_FREE_NODE_NOT_NULLPTR_DATA  ,   ///< .data of free node == nullptr
    LST_BUSY_NODE_NULLPTR_DATA      ,   ///< .data of busy node == nullptr

    LST_NODE_INVALID_NEXT           ,   ///< .next >= capacity
    LST_NODE_INVALID_PREV           ,   ///< .prev >= capacity
    LST_INVALID_CYCLE               ,   ///< нарушен цикл
};

/**
*   @brief Сообщения для каждого типа ошибок
*
*   @see enum LST_STATUS
*/
const char *LST_STATUS_MESSAGES[] = 
{
    "cache_list is OK"                                  ,
    "cache_list is nullptr"                             ,

    "cache_list.fictional is invalid"                   ,
    "cache_list.el_size is invalid"                     ,
    "cache_list.el_free is invalid"                     ,
    "cache_list.size is invalid"                        ,
    "cache_list.capacity is invalid"                    ,
    "cache_list.el_dtor is invalid"                     ,
    "cache_list.el_dump is invalid"                     ,

    "cache_list.fictional is nullptr"                   ,
    "cache_list.fictional->data is not nullptr"         ,
    "cache_list.size >= cache_list.capacity"            ,
    "cache_list.el_free > cache_list.capacity"          ,

    "cache_list_node.data of free node is not nullptr"  ,
    "cache_list_node.data of busy node is nullptr"      ,

    "cache_list_node.next is invalid"                   ,
    "cache_list_node.prev is invalid"                   ,
    "cache_list cycle is invalid"                       ,
};

/**
*   @brief POISON-значения полей листа
*/
static const list LST_POISON =
{
    (list_node *) 0x8BADF00D                    ,   // fictional

    0xBAADF00D                                  ,   // el_size
    0xABADB002                                  ,   // el_free
    0xBADCAB1E                                  ,   // size
    0xABADBABE                                  ,   // capacity

    (void (*) (      void *const)) 0xBADDCAFE   ,   // el_dtor
    (void (*) (const void *const)) 0xBEADFACE   ,   // el_dump
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор кэш-листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned _list_verify(const list *const lst);

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
*   @brief Верификатор полей в свободной вершине кэш-листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned _list_free_node_verify(const list *const lst, const list_node *const lst_node);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор полей в занятой вершине кэш-листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned _list_busy_node_verify(const list *const lst, const list_node *const lst_node);

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конструктор поля fictional кэш-листа.
*   Выделяет динамическую память для вершин кэш-листа, инициализирует фиктивную вершину листа и цикл свободных элементов.
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool list_fictional_ctor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конструктор поля fictional кэш-листа.
*   Инициализирует цикл свободных элементов.
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
*   @param lst [in, out] - указатель на кэш-лист
*
*   @param data    [out] - указатель, куда скопировать содержимое кэш-листа (nullptr по умолчанию)
*   @param ind_cur [in]  - индекс вершины в массиве
*/
static void list_free_node_new(list *const lst, void *const data, const size_t ind_cur);

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
*
*   @param data    [out] - указатель, откуда скопировать содержимое кэш-листа
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*/
static bool list_busy_node_new(list *const lst, const void *const data, const size_t ind_prev,
                                                                        const size_t ind_next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief конструктор занятой вершины кэш-листа.
*
*   @param lst [in, out] - указатель на кэш-лист
*
*   @param data     [in] - указатель, откуда скопировать содержимое кэш-листа
*
*   @param ind_cur  [in] - индекс занятой вершины в массиве вершин
*   @param ind_prev [in] - индекс предыдущей вершины в цикле
*   @param ind_next [in] - индекс следующей вершины в цикле
*/
static bool list_busy_node_ctor(list *const lst, const void *const data, const size_t ind_cur,
                                                                         const size_t ind_prev,
                                                                         const size_t ind_next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Реаллоцирует кэш-лист.
*/
static bool list_resize(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Деструктор содержимого поля .fictional кэш-листа. 
*/
static void list_fictional_dtor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Вызывает .el_dtor или ничего не делает, если поле равно nullptr.
*/
static void list_node_dtor(list *const lst, list_node *const lst_node);

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
*   @brief Дамп кэш-листа.
*
*   @param lst     [in] - указатель на кэш-лист
*   @param is_full [in] - true, если нужен дамп "static" полей
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
*   @param is_full        [in] - true, если нужен дамп "static" полей
*   @param is_any_invalid [in] - true, если хоть одно поле невалидно
*/
static void list_fictional_dump(const list *const lst, const bool is_full,
                                                       const bool is_any_invalid);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Static" дамп массива кэш-листа.
*   Дамп в порядке индексации в массиве, содержит свободные элементы.
*/
static void list_fictional_static_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Static" дамп вершины кэш-листа.
*   Дамп полей вершины с вызовом .el_dump, если .el_dump != nullptr.
*/
static void list_node_static_dump(const list *const lst, const list_node *const lst_node);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief "Public" дамп массива кэш-листа.
*   Дамп в порядке кэш-листа, не содержит свободных элементов.
*/
static void list_fictional_public_dump(const list *const lst);

//================================================================================================================================
// MACRO
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

#if !defined(NVERIFY) && !defined(LIST_NVERIFY)
#define list_verify(lst, ret_val)                                                                                   \
    if (_list_verify(lst) != LST_OK)                                                                                \
    {                                                                                                               \
    $o  return ret_val;                                                                                             \
    }
#else
#define list_verify(lst, ret_val)
#endif

#if !defined(NDEBUG) && !defined(LIST_NDEBUG)
#define list_debug_verify(lst)                                                                                      \
        log_assert(_list_verify(lst) == LST_OK)
#else
#define list_debug_verify(lst)
#endif


#endif //CACHE_LIST_STATIC_H
