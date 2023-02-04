/** @file */
#ifndef LIST_STATIC_H
#define LIST_STATIC_H

#include "list_settings.h"
#include "list.h"

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// list_node
//--------------------------------------------------------------------------------------------------------------------------------

#define $data       lst_node->data
#define $prev       lst_node->prev
#define $next       lst_node->next

//--------------------------------------------------------------------------------------------------------------------------------
// list
//--------------------------------------------------------------------------------------------------------------------------------

#define $fictional  lst->fictional

#define $el_size    lst->el_size
#define $size       lst->size
#define $el_dtor    lst->el_dtor
#define $el_dump    lst->el_dump

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

static unsigned _list_verify(const list *const lst);

static unsigned _list_verify(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_fictional_verify(const list *const lst);

static unsigned _list_fictional_verify(const char *const cur_file,
                                       const char *const cur_func,
                                       const int         cur_line,

                                       const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_data_verify(const list *const lst);

static unsigned _list_data_verify(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_log_error(const list *const lst, const unsigned err);

static void _list_log_error(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_node_verify(const list_node *const lst_node, const bool is_independent);

static unsigned _list_node_verify(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  const list_node *const lst_node, const bool is_independent);

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_data_node_verify(const list_node *const fictional_node,
                                       const list_node *const       cur_node,
                                       const list_node *const      prev_node);

static unsigned _list_data_node_verify(const char *const cur_file,
                                       const char *const cur_func,
                                       const int         cur_line,

                                       const list_node *const fictional_node,
                                       const list_node *const       cur_node,
                                       const list_node *const      prev_node);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_log_error(const list_node *const lst_node, const unsigned err);

static void _list_node_log_error(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list_node *const lst_node, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                              void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

static list *_list_new(const size_t el_size,    void (*el_dtor) (      void *const) = nullptr,
                                                void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_fictional_ctor(list *const lst);

static bool _list_fictional_ctor(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_node_ctor(list      *const lst,
                            list_node *const lst_node,   const void      *const data,
                                                         const list_node *const prev,
                                                         const list_node *const next);

static bool _list_node_ctor(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            list      *const lst,
                            list_node *const lst_node, const void      *const data,
                                                       const list_node *const prev,
                                                       const list_node *const next);

//--------------------------------------------------------------------------------------------------------------------------------

static list_node *_list_node_new(list *const lst,   const void      *const data,
                                                    const list_node *const prev,
                                                    const list_node *const next);


static list_node *_list_node_new(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst, const void      *const data,
                                                  const list_node *const prev,
                                                  const list_node *const next);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

static void _list_dtor(void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_fictional_dtor(list *const lst);

static void _list_fictional_dtor(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_dtor(list *const lst, list_node *const lst_node);

static void _list_node_dtor(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            list *const lst, list_node *const lst_node);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_private_el_dtor(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  void (*el_dtor) (void *const),
                                                   void *const el);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static list_node *_list_get_node(const list *const lst, const size_t index);

static list_node *_list_get_node(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list *const lst, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_insert(list *const lst, const void *const data, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_push_front(list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_push_back(list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_erase(list *const lst, const size_t index, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_pop_front(list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_pop_back(list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_get(const list *const lst, const size_t index, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_front(const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_back(const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

static void _list_dump(const void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_static_dump(const list *const lst, const bool is_full);

static void _list_static_dump(const char *const cur_file,
                              const char *const cur_func,
                              const int         cur_line,

                              const list *const lst, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_public_fields_dump(const list *const lst);

static bool _list_public_fields_dump(const char *const cur_file,
                                     const char *const cur_func,
                                     const int         cur_line,

                                     const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_data_dump(const list *const lst, const bool are_poison_fields, const bool is_full);

static void _list_data_dump(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const bool are_poison_fields, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_fictional_dump(const list *const lst, bool are_poison_fields, const bool is_full);

static bool _list_fictional_dump(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list *const lst, bool are_poison_fields, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_full_fictional_dump(const list *const lst);

static void _list_full_fictional_dump(const char *const cur_file,
                                      const char *const cur_func,
                                      const int         cur_line,

                                      const list *const lst);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_dump(const list *const lst, const list_node *const lst_node, const bool is_full);

static void _list_node_dump(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const list_node *const lst_node, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_private_el_dump(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  void (*el_dump) (const void *const),
                                                   const void *const el);

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_service_fields_dump(const list_node *const lst_node);

static void _list_node_service_fields_dump(const char *const cur_file,
                                           const char *const cur_func,
                                           const int         cur_line,

                                           const list_node *const lst_node);

//================================================================================================================================
// MACRO DEFINITIONS
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

#define list_verify(lst, ret_val)                                                                                   \
    if (_list_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst) != LST_OK)                                       \
    {                                                                                                               \
        return ret_val;                                                                                             \
    }

#ifdef LIST_DEBUG
#define list_debug_verify(lst)                                                                                      \
        log_assert(_list_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst) == LST_OK);
#else
#define list_debug_verify(lst) ;
#endif

#define list_fictional_verify(lst) _list_fictional_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)
#define list_data_verify(     lst) _list_data_verify     (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)

#define list_log_error(lst, err)   _list_log_error       (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, err)

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

#define list_node_verify(lst_node)                                                                                  \
       _list_node_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst_node, false)

#define list_data_node_verify(fictional_node, cur_node, prev_node)                                                  \
       _list_data_node_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, fictional_node, cur_node, prev_node)

#ifdef LIST_DEBUG
#define list_node_debug_verify(lst_node)                                                                            \
    log_assert(_list_node_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst_node, true) == LST_OK);
#else
#define list_node_debug_verify(lst, lst_node) ;
#endif

#define list_node_log_error(lst_node, err)                                                                          \
       _list_node_log_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst_node, err)

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

#define list_fictional_ctor(lst)                                                                                    \
       _list_fictional_ctor(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)

#define list_node_ctor(lst, lst_node, data, prev, next)                                                             \
       _list_node_ctor(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, lst_node, data, prev, next)

#define list_node_new(lst, data, prev, next)                                                                        \
       _list_node_new(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, data, prev, next)

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

#define list_fictional_dtor(lst)          _list_fictional_dtor (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)
#define list_node_dtor(lst, lst_node)     _list_node_dtor      (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, lst_node)
#define list_private_el_dtor(el_dtor, el) _list_private_el_dtor(__FILE__, __PRETTY_FUNCTION__, __LINE__, el_dtor, el)

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

#define list_get_node(lst, index) _list_get_node(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, index)

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

#define list_static_dump(       lst, is_full)                                                                       \
       _list_static_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, is_full)

#define list_public_fields_dump(lst)                                                                                \
       _list_public_fields_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)

#define list_data_dump(lst, are_poison_fields, is_full)                                                             \
       _list_data_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, are_poison_fields, is_full)

#define list_fictional_dump(lst, are_poison_fields, is_full)                                                        \
       _list_fictional_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, are_poison_fields, is_full)

#define list_full_fictional_dump(lst)                                                                               \
       _list_full_fictional_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst)

#define list_node_dump(lst, lst_node, is_full)                                                                      \
       _list_node_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, lst_node, is_full)

#define list_private_el_dump(el_dump, el)                                                                           \
       _list_private_el_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, el_dump, el)

#define list_node_service_fields_dump(lst_node)                                                                     \
       _list_node_service_fields_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst_node)

#endif //LIST_STATIC_H