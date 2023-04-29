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
    "cache_list.size > cache_list.capacity"             ,
    "cache_list.el_free > cache_list.size"              ,

    "cache_list_node.data of free node is not nullptr"  ,
    "cache_list_node.data if busy node is nullptr"      ,

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
