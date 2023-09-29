/** @file */
#ifndef LIST_STATIC_H
#define LIST_STATIC_H

#include <stdio.h>
#include <string.h>

//#define NDEBUG
//#define NVERIFY

#include "list.h"
#include "log.h"

//================================================================================================================================

/**
*   @brief Типы ошибок в листе.
*/
typedef enum
{
    LST_OK                  ,   ///< OK
    LST_NULLPTR             ,   ///< lst = nullptr

    LST_POISON_FICTIONAL    ,   ///< .fictional = LST_POISON.fictional
    LST_POISON_SIZE         ,   ///< .size      = LST_POISON.size
    LST_POISON_EL_SIZE      ,   ///< .el_size   = LST_POISON.el_size
    LST_POISON_EL_DTOR      ,   ///< .el_dtor   = LST_POISON.el_dtor
    LST_POISON_EL_DUMP      ,   ///< .el_dump   = LST_POISON.el_dump

    LST_NULLPTR_FICTIONAL   ,   ///< .fictional = nullptr

    LST_INVALID_NODE        ,   ///< .prev = nullpttr || .next = nullptr
    LST_INVALID_CYCLE       ,   ///< нарушен цикл
}
LST_STATUS_TYPE;

/**
*   @brief Сообщения дл каждого типа ошибок.
*
*   @see enum LST_STATUS_TYPE
*/
static const char *LST_STATUS_MESSAGES[] = 
{
    "list is OK"                ,
    "list is nullptr"           ,

    "list.fictional is poison"  ,
    "list.size"   " is poison"  ,
    "list.el_size"" is poison"  ,
    "list.el_dtor"" is poison"  ,
    "list.el_dump"" is poison"  ,

    "list.fictional is invalid" ,

    "list node"   " is invalid" ,
    "list cycle"  " is invalid" ,
};

static const list LST_POISON =
{
    .fictional = (list_node *) 0xDEADBEEF,

    .size    = 0xBADCAB1E,
    .el_size = 0xABADF00D,

    .el_dtor = (void (*)(      void *)) 0xABADBABE,
    .el_dump = (void (*)(const void *)) 0xABADB002,
};

//================================================================================================================================

static void       list_log_error         (const list *const lst, const unsigned err);
static unsigned   list_fields_verify     (const list *const lst);
static unsigned   list_cycle_verify      (const list *const lst);
static unsigned   list_node_verify       (const list_node *const lst_node);

static bool       list_fictional_ctor    (list *const lst);
static void       list_fictional_dtor    (list *const lst);

static bool       list_node_new          (list *const lst,                            const void *const data, const list_node *const prev, const list_node *const next);
static void       list_node_ctor         (list *const lst, list_node *const lst_node, const void *const data, const list_node *const prev, const list_node *const next);
static bool       list_node_delete       (list *const lst, list_node *const lst_node, void *const erased_data);

static list_node *list_get_node          (const list *const lst, const size_t index);

static void       list_static_dump       (const list *const lst, const bool is_full);
static bool       list_header_dump       (const list *const lst);
static bool       list_public_fields_dump(const list *const lst);
static bool       list_static_fields_dump(const list *const lst);
static void       list_data_dump         (const list *const lst, const bool is_full, const bool is_any_invalid);
static void       list_data_debug_dump   (const list *const lst);
static void       list_data_pretty_dump  (const list *const lst);
static void       list_node_debug_dump   (const list *const lst, const list_node *const lst_node);

#endif // LIST_STATIC_H
