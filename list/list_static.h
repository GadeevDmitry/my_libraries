/** @file */
#ifndef LIST_STATIC_H
#define LIST_STATIC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list_settings.h"
#include "list.h"

//================================================================================================================================
// DSL
//================================================================================================================================

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
#define $size       (lst->size)

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
    LST_POISON_SIZE                 ,   ///< .size             = LST_POISON.size
    LST_POISON_EL_DTOR              ,   ///< .el_dtor          = LST_POISON.el_dtor
    LST_POISON_EL_DUMP              ,   ///< .el_dump          = LST_POISON.el_dump
    LST_NULLPTR_FICTIONAL           ,   ///< .fictional        = nullptr

    LST_FICTIONAL_DATA_NOT_NULLPTR  ,   ///< .fictional->data != nullptr

    LST_NODE_NULLPTR                ,   ///< lst_node          = nullptr
    LST_NODE_NULLPTR_DATA           ,   ///< .data             = nullptr
    LST_NODE_NULLPTR_NEXT           ,   ///< .next             = nullptr
    LST_NODE_NULLPTR_PREV           ,   ///< .prev             = nullptr

    LST_INVALID_CYCLE               ,   ///< нарушен цикл
};

/**
*   @brief Сообщения дл каждого типа ошибок
*
*   @see enum LST_STATUS
*/
const char *LST_STATUS_MESSAGES[] = 
{
    "list is OK"                            ,
    "list is nullptr"                       ,

    "list.fictional is invalid"             ,
    "list.el_size is invalid"               ,
    "list.size is invalid"                  ,
    "list.el_dtor is invalid"               ,
    "list.el_dump is invalid"               ,
    "list.fictional is nullptr"             ,

    "list.fictional->data is not nullptr"   ,

    "list_node is nullptr"                  ,
    "list_node.data is nullptr"             ,
    "list_node.next is nullptr"             ,
    "list_node.prev is nullptr"             ,

    "list cycle is invalid"                 ,
};

/**
*   @brief POISON-значения полей листа
*/
static const list LST_POISON =
{
    (list_node *) 0x8BADF00D                    ,   // fictional

    0xBAADF00D                                  ,   // el_size
    0xBADCAB1E                                  ,   // size

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
*   @brief Верификатор листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned _list_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор поля .fictional листа.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_fictional_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор содержимого листа. Проверка листа на связность.
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_data_verify(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщения об ошибках в листе по битовой маске ошибок. Полный дамп листа.
*/
static void list_log_error(const list *const lst, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор вершины листа.
*   В "независимом" режиме в случае ошибки вызовет _list_node_log_error() для вывода сообщения об ошибке и дампа вершины листа.
*   Иначе просто вернет код ошибки.
*
*   @param lst_node       [in] - вершина листа
*   @param is_independent [in] - true, если "независимый" режим, false иначе
*
*   @return битовая маска кодов ошибок из enum LST_STATUS
*
*   @see enum LST_STATUS
*/
static unsigned list_node_verify(const list_node *const lst_node, const bool is_independent);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Верификатор указателей вершины листа.
*
*   @param fictional_node [in] - фиктивная вершина (поле .fictional) листа, к которому относится данная вершина
*   @param       cur_node [in] - текущая вершина
*   @param      prev_node [in] - предыдущая вершина
*/
static unsigned list_data_node_verify(const list_node *const fictional_node,
                                      const list_node *const       cur_node,
                                      const list_node *const      prev_node);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Выводит сообщения об ошибках в вершине листа по битовой маске ошибок. Дамп вершины листа.
*/
static void list_node_log_error(const list_node *const lst_node, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конструктор поля .fictional листа.
*/
static bool list_fictional_ctor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Конструктор вершины листа.
*
*   @param lst      [in]  - лист, к которому относится данная вершина
*   @param lst_node [out] - вершина листа
*   @param data     [in]  - поле .data вершины листа
*   @param prev     [in]  - поле .prev вершины листа
*   @param next     [in]  - поле .next вершины листа
*
*   @return true, если все ОК, false в случае ошибки
*/
static bool list_node_ctor(list      *const lst,
                           list_node *const lst_node,   const void      *const data,
                                                        const list_node *const prev,
                                                        const list_node *const next);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает вершину листа в динамической памяти.
*
*   @param lst      [in] - лист, к которому относится создаваемая вершина
*   @param data     [in] - поле .data вершины листа
*   @param prev     [in] - поле .prev вершины листа
*   @param next     [in] - поле .next вершины листа
*
*   @return указатель на вершину листа, nullptr в случае ошибки
*/
static list_node *list_node_new(list *const lst, const void      *const data,
                                                 const list_node *const prev,
                                                 const list_node *const next);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Деструктор пользовательских данных листа.
*/
static void list_fictional_dtor(list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Деструктор вершины листа.
*/
static void list_node_dtor(list *const lst, list_node *const lst_node);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает указатель на вершину листа.
*
*   @param lst      [in] - лист
*   @param index    [in] - порядковый номер вершины листа
*
*   @return указатель на вершину листа с порядковым номером index
*/
static list_node *list_get_node(const list *const lst, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп листа.
*   В "полном" режиме, помимо пользовательских данных, дампит и служебные. Используется для дампа невалидного листа.
*
*   @param lst      [in] - лист
*   @param is_full  [in] - true, если "полный" режим, false иначе
*/
static void list_static_dump(const list *const lst, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп полей листа.
*/
static bool list_public_fields_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп содержимого листа.
*   В случае, если хотя бы одно из полей листа является POISON-значением, содержимое выведено не будет.
*
*   @param lst               [in] - лист
*   @param are_poison_fields [in] - true, елси хотя бы одно из полей является POISON-значением, false иначе
*   @param is_full           [in] - true, если "полный" режим
*/
static void list_data_dump(const list *const lst, const bool are_poison_fields, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп поля .fictional листа
*/
static bool list_fictional_dump(const list *const lst, bool are_poison_fields, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп служебных данных поля .fictional листа (указатели на первую и последнюю вершину).
*/
static void list_full_fictional_dump(const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп вершины листа.
*/
static void list_node_dump(const list *const lst, const list_node *const lst_node, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп служебных полей вершины листа (указатели на предыдущую и следующую вершины).
*/
static void list_node_service_fields_dump(const list_node *const lst_node);

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

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

#if !defined(NDEBUG) && !defined(LIST_NDEBUG)
#define list_node_debug_verify(lst_node)                                                                            \
    log_assert(list_node_verify(lst_node, true) == LST_OK);
#else
#define list_node_debug_verify(lst_node)
#endif

#endif //LIST_STATIC_H