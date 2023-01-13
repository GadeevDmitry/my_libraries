#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list_static.h"

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STATIC FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_verify(const list *const lst,  const char *file,
                                                    const char *func,
                                                    const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    unsigned err = LST_OK;

    if (lst == nullptr)
    {
        err = err | (1 << LST_NULLPTR);
        list_log_error(lst, err, file, func, line);
        return err;
    }

    if ($fictional == LST_POISON.fictional) err = err | (1 << LST_POISON_FICTIONAL );
    if ($el_size   == LST_POISON.el_size  ) err = err | (1 << LST_POISON_EL_SIZE   );
    if ($size      == LST_POISON.size     ) err = err | (1 << LST_POISON_SIZE      );
    if ($el_dtor   == LST_POISON.el_dtor  ) err = err | (1 << LST_POISON_EL_DTOR   );
    if ($el_dump   == LST_POISON.el_dump  ) err = err | (1 << LST_POISON_EL_DUMP   );
    if ($fictional ==              nullptr) err = err | (1 << LST_NULLPTR_FICTIONAL);

    if (!( err & (1 << LST_POISON_FICTIONAL) || err & (1 << LST_NULLPTR_FICTIONAL) ))
    {
        err = err | list_fictional_verify(lst);
    }

    list_log_error(lst, err, file, func, line);
    return err;
}

#define list_verify(lst) list_verify(lst, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#ifdef LIST_DEBUG
#define list_debug_verify(lst) if (list_verify(lst) != LST_OK) { assert(false && "list verify failed\n"); }
#else
#define list_debug_verify(lst) ;
#endif

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_fictional_verify(const list *const lst)
{
    assert(lst        != nullptr);
    assert($fictional != nullptr);

    const list_node *cur_node = $fictional->next;
    unsigned err = 0;

    for (size_t i = 0; i < $size; ++i)
    {
        err = err | list_node_verify(lst, cur_node);

        if (err) break;
        if (cur_node->prev->next != cur_node) { err = err | (1 << LST_INVALID_CYCLE); break; }
        if (cur_node == $fictional)           { err = err | (1 << LST_INVALID_CYCLE); break; }

        cur_node = cur_node->next;
    }

    if (cur_node != $fictional) err = err | (1 << LST_INVALID_CYCLE);

    return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_log_error(const list *const lst, const unsigned err,   const char *file,
                                                                        const char *func,
                                                                        const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    if (err == LST_OK) return;

    log_param_error(file, func, line, "list verify failed\n");
    log_message(HTML_COLOR_DARK_RED);

    for (size_t i = 1; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_message("%s\n", LST_STATUS_MESSAGES[i]);
    }
    log_message(HTML_COLOR_CANCEL);

    list_static_dump(lst, file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_node_verify(const list *const lst, const list_node *const lst_node)
{
    assert(lst != nullptr);

    unsigned err = 0;
    if (lst_node == nullptr)
    {
        err = err | (1 << LST_NODE_NULLPTR);
        return err;
    }

    if ($data == nullptr) err = err | (1 << LST_NODE_NULLPTR_DATA);
    if ($prev == nullptr) err = err | (1 << LST_NODE_NULLPTR_PREV);
    if ($next == nullptr) err = err | (1 << LST_NODE_NULLPTR_NEXT);

    return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

#ifdef LIST_DEBUG

static unsigned list_node_debug_verify(const list *const lst, const list_node *const lst_node,  const char *file,
                                                                                                const char *func,
                                                                                                const int   line)
{
    assert(lst  != nullptr);
    assert(file != nullptr);
    assert(func != nullptr);

    unsigned err = list_node_verify(lst, lst_node);

    list_node_log_error(lst, lst_node, err, file, func, line);
    return err;
}

#define list_node_debug_verify(lst, lst_node)                                                                                   \
    if (list_node_debug_verify(lst, lst_node, __FILE__, __PRETTY_FUNCTION__, __LINE__) != LST_OK)                               \
    {                                                                                                                           \
        assert(false && "list_node verify failed\n");                                                                           \
    }
#else
#define list_node_debug_verify(lst, lst_node) ;
#endif //LIST_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------

#ifdef LIST_DEBUG

static void list_node_log_error(const list      *const lst,
                                const list_node *const lst_node, const unsigned err,    const char *file,
                                                                                        const char *func,
                                                                                        const int   line)
{
    assert(lst  != nullptr);
    assert(file != nullptr);
    assert(func != nullptr);

    if (err == 0) return;

    log_param_error(file, func, line, "%s\n", "list_node verify failed\n");
    log_message(HTML_COLOR_DARK_RED);

    for (size_t i = 0; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_message("%s\n", LST_STATUS_MESSAGES[i]);
    }
    log_message(HTML_COLOR_CANCEL);

    list_node_static_dump(lst, lst_node, file, func, line);
}

#endif //LIST_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

static void list_static_dump(const list *const lst, const char *file,
                                                    const char *func,
                                                    const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("list_static_dump called by error in\n"
                    "    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n\n", file, func, line);

    list_public_fields_dump(lst);
    if (lst == nullptr) return;

    list_fictional_dump(lst);

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

#define poison_field_dump(field)                         log_tab_message(field " = " HTML_COLOR_POISON      "POISON"       HTML_COLOR_CANCEL "\n")

#define good_field_dump(   field, specified_char, value) log_tab_message(field " = " HTML_COLOR_LIME_GREEN  specified_char HTML_COLOR_CANCEL "\n", value)
#define warning_field_dump(field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_ORANGE specified_char HTML_COLOR_CANCEL "\n", value)
#define error_field_dump(  field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_RED    specified_char HTML_COLOR_CANCEL "\n", value)
#define usual_field_dump(  field, specified_char, value) log_tab_message(field " = "                        specified_char                   "\n", value)

static void list_public_fields_dump(const list *const lst)
{
    log_tab_message("list (address: %p)\n"
                    "{\n",          lst);
    LOG_TAB++;

    if (lst == nullptr) { LOG_TAB--; log_tab_message("}\n"); return; }

    if ($el_size == LST_POISON.el_size) poison_field_dump ("el_size");
    else                                usual_field_dump  ("el_size", "%lu", $el_size);

    if ($el_dtor == LST_POISON.el_dtor) poison_field_dump ("el_dtor");
    else if ($el_dtor == nullptr)       warning_field_dump("el_dtor", "%p",   nullptr);
    else                                usual_field_dump  ("el_dtor", "%p",  $el_dtor);

    if ($el_dump == LST_POISON.el_dump) poison_field_dump ("el_dump");
    else if ($el_dump == nullptr)       warning_field_dump("el_dump", "%p",   nullptr);
    else                                usual_field_dump  ("el_dump", "%p",   nullptr);

    if ($size == LST_POISON.size)       poison_field_dump ("size   ");
    else                                usual_field_dump  ("size   ", "%lu",    $size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dump(const list *const lst)
{
    assert(lst != nullptr);

    log_tab_message("fictional = %p\n"
                    "{\n", $fictional);
    LOG_TAB++;

    if ($fictional == nullptr) { LOG_TAB--; log_tab_message("}\n"); return; }

    log_message(HTML_COLOR_MEDIUM_BLUE);
    list_node_public_fields_dump($fictional);
    if ($fictional->data != nullptr) error_field_dump("data", "%p", $fictional->data);
    else                             good_field_dump ("data", "%p", $fictional->data);
    log_message(HTML_COLOR_CANCEL);

    const list_node *cur_node = $fictional;
    for (size_t i = 0; i < $size; ++i)
    {
        if (cur_node == nullptr) break;
        cur_node = cur_node->next;

        log_tab_message("%lu (address: %p):\n", i, cur_node);
        log_tab_message("{\n");
        LOG_TAB++;

        list_node_static_dump(lst, cur_node);

        LOG_TAB--;
        log_tab_message("}\n");
    }

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_public_fields_dump(const list_node *const lst_node)
{
    assert(lst_node != nullptr);

    if ($prev == nullptr || $prev->next != lst_node) error_field_dump("prev", "%p", $prev);
    else                                             good_field_dump ("prev", "%p", $prev);

    if ($next == nullptr || $next->prev != lst_node) error_field_dump("next", "%p", $next);
    else                                             good_field_dump ("next", "%p", $next);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_static_dump(const list *const lst, const list_node *const lst_node)
{
    assert(lst != nullptr);

    list_node_public_fields_dump(lst_node);
    if ($data == nullptr) error_field_dump("data", "%p", nullptr);
    else
    {
        usual_field_dump("data", "%p", $data);
        if ($el_dump == nullptr) { log_warning("can't dump list elements\n"); }
        else                     { (*$el_dump)($data); }
    }
}

#ifdef LIST_DEBUG

static void list_node_static_dump(const list      *const lst,
                                  const list_node *const lst_node,  const char *file,
                                                                    const char *func,
                                                                    const int   line)
{
    assert(lst  != nullptr);
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("list_node_static_dump called by error in\n"
                    "    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n\n", file, func, line);

    list_node_static_dump(lst, lst_node);
}

#endif //LIST_DEBUG

//================================================================================================================================
// USER FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                      void (*el_dump) (const void *const) /* = nullptr */)
{
    if (lst == nullptr) { list_log_error(lst, LST_NULLPTR, __FILE__, __PRETTY_FUNCTION__, __LINE__); return false; }

    $el_size = el_size;
    $size    =       0;
    $el_dtor = el_dtor;
    $el_dump = el_dump;

    if (!list_fictional_ctor(lst)) return false;

    list_debug_verify(lst);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

list *list_new(const size_t el_size,    void (*el_dtor) (      void *const) /* = nullptr */,
                                        void (*el_dump) (const void *const) /* = nullptr */)
{
    list *lst = (list *) log_calloc(1, sizeof(list));
    if (lst == nullptr)
    {
        log_error("log_calloc(1, sizeof(list)) returns nullptr\n");
        return nullptr;
    }
    if (!list_ctor(lst, el_size, el_dtor, el_dump)) { log_free(lst); return nullptr; }

    list_debug_verify(lst);
    return lst;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_fictional_ctor(list *const lst)
{
    assert(lst != nullptr);

    $fictional = (list_node *) log_calloc(1, sizeof(list_node));
    if ($fictional == nullptr)
    {
        log_error("log_calloc(1, sizeof(list_node)) = nullptr\n");
        return false;
    }

    $fictional->data =    nullptr;
    $fictional->prev = $fictional;
    $fictional->next = $fictional;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_node_ctor(list      *const lst,
                           list_node *const lst_node,   const void      *const data,
                                                        const list_node *const prev,
                                                        const list_node *const next)
{
    list_debug_verify(lst);
    assert(lst_node != nullptr);

    $data = log_calloc(1, $el_size);
    if ($data == nullptr)
    {
        log_error("log_calloc(1, %d /* = el_size */) returns nullptr\n", $el_size);
        return false;
    }

    memcpy($data, data, $el_size);
    $prev = (list_node *) prev;
    $next = (list_node *) next;

    list_node_debug_verify(lst, lst_node);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_node_new(list *const lst,    const void      *const data,
                                                    const list_node *const prev,
                                                    const list_node *const next)
{
    list_node *lst_node = (list_node *) log_calloc(1, sizeof(list_node));
    if (lst_node == nullptr)
    {
        log_error("log_calloc(1, sizeof(list_node)) returns nullptr\n");
        return nullptr;
    }

    if (!list_node_ctor(lst, lst_node, data, prev, next)) { log_free(lst_node); return nullptr; }

    list_node_debug_verify(lst, lst_node);
    return lst_node;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void list_dtor(void *const _lst)
{
    list *const lst = (list *) _lst;
    if (list_verify(lst) != LST_OK) return;

    list_fictional_dtor(lst);
    *lst = LST_POISON;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dtor(list *const lst)
{
    assert(lst        != nullptr);
    assert($fictional != nullptr);

    list_node *cur_node = $fictional->next;
    for (; cur_node != $fictional;)
    {
        list_node *next_node = cur_node->next;
        list_node_dtor(lst, cur_node);

        cur_node = next_node;
    }
    log_free($fictional);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_dtor(list *const lst, list_node *const lst_node)
{
    assert(lst      != nullptr);
    assert(lst_node != nullptr);

    if ($el_dtor == nullptr) { log_warning("can't dtor list elements\n"); }
    else (*$el_dtor) ($data);

    log_free($data);
    log_free(lst_node);
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_get_node(const list *const lst, const size_t index)
{
    list_debug_verify(lst);
    assert(index <  $size);

    list_node *cur_node = $fictional;
    for (size_t i = 0; i <= index; ++i) cur_node = cur_node->next;

    return cur_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_insert(list *const lst, const void *const data, const size_t index)
{
    if (list_verify(lst) != LST_OK) return false;
    if (data == nullptr) { log_error("param data is nullptr");             return false; }
    if (index >   $size) { log_error("param index more than list size\n"); return false; }

    list_node *prev_node = nullptr;
    list_node *next_node = nullptr;

    if (index == 0)
    {
        prev_node = $fictional;
        next_node = $fictional->next;
    }
    else
    {
        prev_node = list_get_node(lst, index - 1);
        next_node = prev_node->next;
    }

    list_node *cur_node = list_node_new(lst, data, prev_node, next_node);
    if (cur_node == nullptr) return false;

    prev_node->next = cur_node;
    next_node->prev = cur_node;
    $size += 1;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_push_front(list *const lst, const void *const data)
{
    return list_insert(lst, data, 0);
}

bool list_push_back(list *const lst, const void *const data)
{
    return list_insert(lst, data, $size);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_erase(list *const lst, const size_t index, void *const data /* = nullptr */)
{
    if (list_verify(lst) != LST_OK) return false;
    if (index >= $size) { log_error("param index equal or more than list size\n"); return false; }

    list_node *cur_node = list_get_node(lst, index);
    if (data != nullptr) memcpy(data, cur_node->data, $el_size);

    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    $size -= 1;

    list_node_dtor(lst, cur_node);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_front(list *const lst, void *const data /* = nullptr */)
{
    return list_erase(lst, 0, data);
}

bool list_pop_back(list *const lst, void *const data /* = nullptr */)
{
    return list_erase(lst, $size - 1, data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

bool list_get(const list *const lst, const size_t index, void *const data)
{
    list_verify(lst);
    if (index >=   $size) { log_error("param index equql or more than list size\n"); return false; }
    if (data  == nullptr) { log_error("param data is nullptr\n");                    return false; }

    const list_node *const lst_node = list_get_node(lst, index);
    memcpy(data, $data, $el_size);

    list_debug_verify(lst);
    return true;
}

bool list_front(const list *const lst, void *const data)
{
    return list_get(lst, 0, data);
}

bool list_back(const list *const lst, void *const data)
{
    return list_get(lst, $size - 1, data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void list_dump(const void *const _lst)
{
    const list *const lst = (const list *) _lst;

    list_public_fields_dump(lst);
    if (lst == nullptr) return;

    list_fictional_dump(lst);

    LOG_TAB--;
    log_tab_message("}\n");
}
