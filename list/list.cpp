#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_verify(const list *const lst)
{
    unsigned err = LST_OK;

    if (lst == nullptr)
    {
        err = err | (1 << LST_NULLPTR);
        list_log_error(lst, err);
        return err;
    }

    if ($el_size   == LST_POISON.el_size  ) err = err | (1 << LST_POISON_EL_SIZE   );
    if ($size      == LST_POISON.size     ) err = err | (1 << LST_POISON_SIZE      );
    if ($el_dtor   == LST_POISON.el_dtor  ) err = err | (1 << LST_POISON_EL_DTOR   );
    if ($el_dump   == LST_POISON.el_dump  ) err = err | (1 << LST_POISON_EL_DUMP   );

    err = err | list_fictional_verify(lst);

    if (err == LST_OK)                      err = err | list_data_verify(lst);

    list_log_error(lst, err);
    return err;
}

static unsigned _list_verify(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _list_verify(lst);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_fictional_verify(const list *const lst)
{
    log_assert(lst != nullptr);

    unsigned err = LST_OK;

    if ($fictional == LST_POISON.fictional)
    {
        err = err | (1 << LST_POISON_FICTIONAL);
        return err;
    }
    if ($fictional == nullptr)
    {
        err = err | (1 << LST_NODE_NULLPTR);
        return err;
    }

    if ($fictional->data != nullptr) err = err | (1 << LST_FICTIONAL_DATA_NOT_NULLPTR);
    if ($fictional->prev == nullptr) err = err | (1 << LST_NODE_NULLPTR_PREV);
    if ($fictional->next == nullptr) err = err | (1 << LST_NODE_NULLPTR_NEXT);

    return err;
}

static unsigned _list_fictional_verify(const char *const cur_file,
                                       const char *const cur_func,
                                       const int         cur_line,

                                       const list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _list_fictional_verify(lst);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_data_verify(const list *const lst)
{
    log_assert(lst        !=              nullptr);
    log_assert($fictional !=              nullptr);
    log_assert($fictional != LST_POISON.fictional);

    unsigned err = LST_OK;

    const list_node *cur_node  = $fictional->next;
    const list_node *prev_node = $fictional;

    for (size_t i = 0; i < $size; ++i)
    {
        err = err | list_data_node_verify($fictional, cur_node, prev_node);

        if (err != LST_OK && err != LST_NODE_NULLPTR_DATA) break;

        prev_node = cur_node;
        cur_node  = cur_node->next;
    }

    if (cur_node != $fictional) err = err | (1 << LST_INVALID_CYCLE);

    return err;
}

static unsigned _list_data_verify(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  const list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _list_data_verify(lst);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_log_error(const list *const lst, const unsigned err)
{
    if (err == LST_OK) return;

    log_error("list verify failed\n");

    for (size_t i = 1; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_error_message("%s\n", LST_STATUS_MESSAGES[i]);
    }

    list_static_dump(lst, true);
    log_message("\n");
}

static void _list_log_error(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const unsigned err)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_log_error(lst, err);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_node_verify(const list_node *const lst_node, const bool is_independent)
{
    unsigned err = 0;

    if (lst_node == nullptr)
    {
        err = err | (1 << LST_NODE_NULLPTR);
        if (is_independent) list_node_log_error(lst_node, err);

        return err;
    }

    if ($data == nullptr) err = err | (1 << LST_NODE_NULLPTR_DATA);
    if ($prev == nullptr) err = err | (1 << LST_NODE_NULLPTR_PREV);
    if ($next == nullptr) err = err | (1 << LST_NODE_NULLPTR_NEXT);

    if (is_independent) list_node_log_error(lst_node, err);

    return err;
}

static unsigned _list_node_verify(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  const list_node *const lst_node, const bool is_independent)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _list_node_verify(lst_node, is_independent);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_data_node_verify(const list_node *const fictional_node,
                                       const list_node *const       cur_node,
                                       const list_node *const      prev_node)
{
    log_assert(fictional_node != nullptr);
    log_assert(cur_node       != nullptr);
    log_assert(prev_node      != nullptr);

    unsigned err = LST_OK;

    if (cur_node == fictional_node) return err = err | (1 << LST_INVALID_CYCLE);

                                     err = err | list_node_verify(cur_node);
    if (cur_node->prev != prev_node) err = err | (1 << LST_INVALID_CYCLE);

    return err;
}

static unsigned _list_data_node_verify(const char *const cur_file,
                                       const char *const cur_func,
                                       const int         cur_line,

                                       const list_node *const fictional_node,
                                       const list_node *const       cur_node,
                                       const list_node *const      prev_node)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _list_data_node_verify(fictional_node, cur_node, prev_node);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_log_error(const list_node *const lst_node, const unsigned err)
{
    if (err == LST_OK) return;

    log_error("list_node verify failed\n");

    for (size_t i = 0; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_error_message("%s\n", LST_STATUS_MESSAGES[i]);
    }

    list_node_service_fields_dump(lst_node);
    log_message("\n");
}

static void _list_node_log_error(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list_node *const lst_node, const unsigned err)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_node_log_error(lst_node, err);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                              void (*el_dump) (const void *const) /* = nullptr */)
{
    log_verify(lst != nullptr, false);

    $el_size = el_size;
    $size    =       0;
    $el_dtor = el_dtor;
    $el_dump = el_dump;

    if (!list_fictional_ctor(lst)) return false;

    list_debug_verify(lst);
    return true;
}

bool _list_ctor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                       void (*el_dump) (const void *const) /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_ctor(lst, el_size, el_dtor, el_dump);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static list *_list_new(const size_t el_size,    void (*el_dtor) (      void *const) /* = nullptr */,
                                                void (*el_dump) (const void *const) /* = nullptr */)
{
    list *lst = (list *) log_calloc(1, sizeof(list));
    if (lst == nullptr)
    {
        log_error("log_calloc(1, sizeof(list) = %lu) returns nullptr\n", sizeof(list));
        return nullptr;
    }
    if (!list_ctor(lst, el_size, el_dtor, el_dump)) { log_free(lst); return nullptr; }

    list_debug_verify(lst);
    return lst;
}

list *_list_new(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                      void (*el_dump) (const void *const) /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    list *ret = _list_new(el_size, el_dtor, el_dump);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_fictional_ctor(list *const lst)
{
    log_assert(lst != nullptr);

    $fictional = (list_node *) log_calloc(1, sizeof(list_node));
    if ($fictional == nullptr)
    {
        log_error("log_calloc(1, sizeof(list_node) = %lu) returns nullptr\n", sizeof(list_node));
        return false;
    }

    $fictional->data =    nullptr;
    $fictional->prev = $fictional;
    $fictional->next = $fictional;

    return true;
}

static bool _list_fictional_ctor(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_fictional_ctor(lst);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_node_ctor(list      *const lst,
                            list_node *const lst_node,   const void      *const data,
                                                         const list_node *const prev,
                                                         const list_node *const next)
{
    list_debug_verify(lst);
    log_assert(lst_node != nullptr);

    $data = log_calloc(1, $el_size);
    if ($data == nullptr)
    {
        log_error("log_calloc(1, el_size = %lu) returns nullptr\n", $el_size);
        return false;
    }

    memcpy($data, data, $el_size);
    $prev = (list_node *) prev;
    $next = (list_node *) next;

    list_node_debug_verify(lst_node);
    return true;
}

static bool _list_node_ctor(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            list      *const lst,
                            list_node *const lst_node, const void      *const data,
                                                       const list_node *const prev,
                                                       const list_node *const next)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_node_ctor(lst, lst_node, data, prev, next);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static list_node *_list_node_new(list *const lst,   const void      *const data,
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

    list_node_debug_verify(lst_node);
    return lst_node;
}

static list_node *_list_node_new(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst, const void      *const data,
                                                  const list_node *const prev,
                                                  const list_node *const next)
{
    trace_push(cur_file, cur_func, cur_line);
    list_node *ret = _list_node_new(lst, data, prev, next);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

static void _list_dtor(void *const _lst)
{
    list *const lst = (list *) _lst;
    list_verify(lst, ;);

    list_fictional_dtor(lst);
    *lst = LST_POISON;
}

void _list_dtor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                void *const _lst)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_dtor(_lst);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_fictional_dtor(list *const lst)
{
    list_debug_verify(lst);
    log_assert($fictional != nullptr);

    list_node *cur_node = $fictional->next;
    if (cur_node != $fictional && $el_dtor == nullptr) { log_warning("can't dtor list elements\n"); }

    for (; cur_node != $fictional;)
    {
        list_node *next_node = cur_node->next;
        list_node_dtor(lst, cur_node);

        cur_node = next_node;
    }
    log_free($fictional);
}

static void _list_fictional_dtor(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_fictional_dtor(lst);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_dtor(list *const lst, list_node *const lst_node)
{
    log_assert(lst      != nullptr);
    log_assert(lst_node != nullptr);

    if ($el_dtor != nullptr) list_private_el_dtor($el_dtor, $data);

    log_free($data);
    log_free(lst_node);
}

static void _list_node_dtor(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            list *const lst, list_node *const lst_node)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_node_dtor(lst, lst_node);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_private_el_dtor(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  void (*el_dtor) (void *const),
                                                   void *const el)
{
    log_assert(el_dtor != nullptr);

    trace_push(cur_file, cur_func, cur_line);
    (*el_dtor)(el);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static list_node *_list_get_node(const list *const lst, const size_t index)
{
    list_debug_verify(lst);
    log_verify(index < $size, nullptr);

    list_node *cur_node = $fictional;

    if (index <= $size / 2)
    {
        for (size_t i = 0; i <= index; ++i) cur_node = cur_node->next;
    }
    else
    {
        for (size_t i = $size; i > index; --i) cur_node = cur_node->prev;
    }

    list_debug_verify(lst);

    return cur_node;
}

static list_node *_list_get_node(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list *const lst, const size_t index)
{
    trace_push(cur_file, cur_func, cur_line);
    list_node *ret = _list_get_node(lst, index);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_insert(list *const lst, const void *const data, const size_t index)
{
    list_verify(lst,              false);
    log_verify (data  != nullptr, false);
    log_verify (index <=   $size, false);

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

    list_debug_verify(lst);

    return true;
}

bool _list_insert(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  list *const lst, const void *const data, const size_t index)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_insert(lst, data, index);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_push_front(list *const lst, const void *const data)
{
    list_verify(lst, false);
    return list_insert(lst, data, 0);
}

bool _list_push_front(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      list *const lst, const void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_push_front(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_push_back(list *const lst, const void *const data)
{
    list_verify(lst, false);
    return list_insert(lst, data, $size);
}

bool _list_push_back(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, const void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_push_back(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_erase(list *const lst, const size_t index, void *const data /* = nullptr */)
{
    list_verify(lst,           false);
    log_verify (index < $size, false);

    list_node *cur_node = list_get_node(lst, index);
    if (data != nullptr) memcpy(data, cur_node->data, $el_size);

    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    $size -= 1;

    list_node_dtor(lst, cur_node);
    list_debug_verify(lst);

    return true;
}

bool _list_erase(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 list *const lst, const size_t index, void *const data /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_erase(lst, index, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_pop_front(list *const lst, void *const data /* = nullptr */)
{
    list_verify(lst, false);
    return list_erase(lst, 0, data);
}

bool _list_pop_front(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, void *const data /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_pop_front(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_pop_back(list *const lst, void *const data /* = nullptr */)
{
    list_verify(lst, false);
    return list_erase(lst, $size - 1, data);
}

bool _list_pop_back(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    list *const lst, void *const data /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_pop_back(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_get(const list *const lst, const size_t index, void *const data)
{
    list_verify(lst,             false);
    log_verify (index <   $size, false);
    log_verify (data != nullptr, false);

    const list_node *const lst_node = list_get_node(lst, index);
    memcpy(data, $data, $el_size);

    list_debug_verify(lst);

    return true;
}

bool _list_get(const char *const cur_file,
               const char *const cur_func,
               const int         cur_line,

               const list *const lst, const size_t index, void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_get(lst, index, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_front(const list *const lst, void *const data)
{
    list_verify(lst, false);
    return list_get(lst, 0, data);
}

bool _list_front(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const list *const lst, void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_front(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_back(const list *const lst, void *const data)
{
    list_verify(lst, false);
    return list_get(lst, $size - 1, data);
}

bool _list_back(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const list *const lst, void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_back(lst, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

static void _list_dump(const void *const _lst)
{
    const list *const lst = (const list *) _lst;
    list_verify(lst, ;);

    list_static_dump(lst, false);
}

void _list_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const void *const _lst)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_dump(_lst);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_static_dump(const list *const lst, const bool is_full)
{
    bool are_poison_fields = list_public_fields_dump(lst);
    if (lst == nullptr) return;

    list_data_dump(lst, are_poison_fields, is_full);

    LOG_TAB--;
    log_tab_message("}\n");
}

static void _list_static_dump(const char *const cur_file,
                              const char *const cur_func,
                              const int         cur_line,

                              const list *const lst, const bool is_full)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_static_dump(lst, is_full);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_public_fields_dump(const list *const lst)
{
    bool is_any_poison = false;

    log_tab_message("list (address: %p)\n"
                    "{\n",          lst);
    LOG_TAB++;

    if (lst == nullptr) { LOG_TAB--; log_tab_message("}\n"); return is_any_poison; }

    if      ($el_size == LST_POISON.el_size) { poison_field_dump ("el_size  "); is_any_poison = true; }
    else                                     { usual_field_dump  ("el_size  ", "%lu", $el_size);      }

    if      ($el_dtor == LST_POISON.el_dtor) { poison_field_dump ("el_dtor  "); is_any_poison = true; }
    else if ($el_dtor == nullptr)            { warning_field_dump("el_dtor  ", "%p",   nullptr);      }
    else                                     { usual_field_dump  ("el_dtor  ", "%p",  $el_dtor);      }

    if      ($el_dump == LST_POISON.el_dump) { poison_field_dump ("el_dump  "); is_any_poison = true; }
    else if ($el_dump == nullptr)            { warning_field_dump("el_dump  ", "%p",   nullptr);      }
    else                                     { usual_field_dump  ("el_dump  ", "%p",  $el_dump);      }

    if      ($size    == LST_POISON.size)    { poison_field_dump ("size     "); is_any_poison = true; }
    else                                     { usual_field_dump  ("size     ", "%lu",    $size);      }

    return is_any_poison;
}

static bool _list_public_fields_dump(const char *const cur_file,
                                     const char *const cur_func,
                                     const int         cur_line,

                                     const list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_public_fields_dump(lst);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_data_dump(const list *const lst, const bool are_poison_fields, const bool is_full)
{
    log_assert(lst != nullptr);

    if (!list_fictional_dump(lst, are_poison_fields, is_full)) return;

    log_tab_message("{\n"); LOG_TAB++;

    const list_node *cur_node = $fictional->next;

    for (size_t i = 0; i < $size; ++i)
    {
        if (cur_node == nullptr)    break;
        if (cur_node == $fictional) break;

        log_tab_message("#%lu:\n", i);
        list_node_dump(lst, cur_node, is_full);
        cur_node = cur_node->next;
    }

    LOG_TAB--;
    log_tab_message("}\n");
}

static void _list_data_dump(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const bool are_poison_fields, const bool is_full)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_data_dump(lst, are_poison_fields, is_full);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _list_fictional_dump(const list *const lst, bool are_poison_fields, const bool is_full)
{
    log_assert(lst != nullptr);

    if      ($fictional == LST_POISON.fictional) { poison_field_dump("fictional"); are_poison_fields = true; }
    else if ($fictional == nullptr)              { error_field_dump ("fictional", "%p", $fictional); }
    else                                         { usual_field_dump ("fictional", "%p", $fictional); }

    if ($fictional == nullptr) return false;

    if (are_poison_fields)
    {
        log_tab_message("{\n"); LOG_TAB++;
        log_error_message("Some of list's fields are POISON. Can't dump list content\n");
        LOG_TAB--; log_tab_message("}\n");

        return false;
    }

    if (is_full) list_full_fictional_dump(lst);

    return true;
}

static bool _list_fictional_dump(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const list *const lst, bool are_poison_fields, const bool is_full)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _list_fictional_dump(lst, are_poison_fields, is_full);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_full_fictional_dump(const list *const lst)
{
    log_assert(lst != nullptr);
    log_assert($fictional != nullptr);
    log_assert($fictional != LST_POISON.fictional);

    log_tab_message("{" HTML_COLOR_MEDIUM_BLUE "\n"); LOG_TAB++;

    if ($fictional->prev == nullptr) error_field_dump("prev", "%p", $fictional->prev);
    else                             usual_field_dump("prev", "%p", $fictional->prev);

    if ($fictional->next == nullptr) error_field_dump("next", "%p", $fictional->next);
    else                             usual_field_dump("next", "%p", $fictional->next);

    if ($fictional->data != nullptr) error_field_dump("data", "%p", $fictional->data);
    else                             good_field_dump ("data", "%p", $fictional->data);

    LOG_TAB--; log_tab_message(HTML_COLOR_CANCEL "}\n");

}

static void _list_full_fictional_dump(const char *const cur_file,
                                      const char *const cur_func,
                                      const int         cur_line,

                                      const list *const lst)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_full_fictional_dump(lst);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_dump(const list *const lst, const list_node *const lst_node, const bool is_full)
{
    if (is_full) list_node_service_fields_dump(lst_node);

    log_tab_message("{\n"); LOG_TAB++;

    if ($data != nullptr)
    {
        if ($el_dump == nullptr) { log_warning_message("can't dump list elements\n"); }
        else                     { list_private_el_dump($el_dump, $data); }
    }

    LOG_TAB--; log_tab_message("}\n");
}

static void _list_node_dump(const char *const cur_file,
                            const char *const cur_func,
                            const int         cur_line,

                            const list *const lst, const list_node *const lst_node, const bool is_full)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_node_dump(lst, lst_node, is_full);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_private_el_dump(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  void (*el_dump) (const void *const),
                                                   const void *const el)
{
    log_assert(el_dump != nullptr);
    log_assert(el      != nullptr);

    trace_push(cur_file, cur_func, cur_line);
    (*el_dump)(el);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _list_node_service_fields_dump(const list_node *const lst_node)
{
    log_assert(lst_node != nullptr);

    log_tab_message(HTML_COLOR_MEDIUM_BLUE
                    "list_node (address: %p)\n"
                    "{\n", lst_node);
    LOG_TAB++;

    if      ($prev == nullptr)        error_field_dump("prev", "%p", $prev);
    else if ($prev->next != lst_node) error_field_dump("prev", "%p", $prev);
    else                              usual_field_dump("prev", "%p", $prev);

    if      ($next == nullptr)        error_field_dump("next", "%p", $next);
    else if ($next->prev != lst_node) error_field_dump("next", "%p", $next);
    else                              usual_field_dump("next", "%p", $next);

    if      ($data == nullptr)        error_field_dump("data", "%p", $data);
    else                              usual_field_dump("data", "%p", $data);

    LOG_TAB--;
    log_tab_message("}" HTML_COLOR_CANCEL "\n");
}

static void _list_node_service_fields_dump(const char *const cur_file,
                                           const char *const cur_func,
                                           const int         cur_line,

                                           const list_node *const lst_node)
{
    trace_push(cur_file, cur_func, cur_line);
    _list_node_service_fields_dump(lst_node);
    trace_pop();
}