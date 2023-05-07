#include "list_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static unsigned _list_verify(const list *const lst)
{
$i
    unsigned err = LST_OK;

    if (lst == nullptr)
    {
        err = err | (1 << LST_NULLPTR);
$       list_log_error(lst, err);
$o      return err;
    }

    if ($size    == LST_POISON.size   ) err = err | (1 << LST_POISON_SIZE   );
    if ($el_dump == LST_POISON.el_dump) err = err | (1 << LST_POISON_EL_DUMP);

$   /* fictional */    err = err | list_fictional_verify(lst);
$   if (err == LST_OK) err = err | list_data_verify(lst);

$   list_log_error(lst, err);
$o  return err;
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_fictional_verify(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    unsigned err = LST_OK;

    if ($fictional == LST_POISON.fictional)
    {
        err = err | (1 << LST_POISON_FICTIONAL);
$o      return err;
    }
    if ($fictional == nullptr)
    {
        err = err | (1 << LST_NODE_NULLPTR);
$o      return err;
    }

    if ($fictional->data != nullptr) err = err | (1 << LST_FICTIONAL_DATA_NOT_NULLPTR);
    if ($fictional->prev == nullptr) err = err | (1 << LST_NODE_NULLPTR_PREV);
    if ($fictional->next == nullptr) err = err | (1 << LST_NODE_NULLPTR_NEXT);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_data_verify(const list *const lst)
{
$i
    log_assert(lst        !=              nullptr);
    log_assert($fictional !=              nullptr);
    log_assert($fictional != LST_POISON.fictional);

    unsigned err = LST_OK;

    const list_node *cur_node  = $fictional->next;
    const list_node *prev_node = $fictional;

$   for (size_t i = 0; i < $size; ++i)
    {
        err = err | list_data_node_verify($fictional, cur_node, prev_node);

        if (err != LST_OK && err != LST_NODE_NULLPTR_DATA) break;

        prev_node = cur_node;
        cur_node  = cur_node->next;
    }

    if (cur_node != $fictional) err = err | (1 << LST_INVALID_CYCLE);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_log_error(const list *const lst, const unsigned err)
{
$i
    if (err == LST_OK) { $o return; }

$   log_error("list verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) { log_tab_error_message("%s", "\n", LST_STATUS_MESSAGES[i]); }
    }

$   list_static_dump(lst, true);
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// list_node verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_node_verify(const list_node *const lst_node, const bool is_independent)
{
$i
    unsigned err = 0;

    if (lst_node == nullptr)
    {
        err = err | (1 << LST_NODE_NULLPTR);
$       if (is_independent) list_node_log_error(lst_node, err);

$o     return err;
    }

    if ($data == nullptr) err = err | (1 << LST_NODE_NULLPTR_DATA);
    if ($prev == nullptr) err = err | (1 << LST_NODE_NULLPTR_PREV);
    if ($next == nullptr) err = err | (1 << LST_NODE_NULLPTR_NEXT);

$   if (is_independent) list_node_log_error(lst_node, err);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_data_node_verify(const list_node *const fictional_node,
                                      const list_node *const       cur_node,
                                      const list_node *const      prev_node)
{
$i
    log_assert(fictional_node != nullptr);
    log_assert(cur_node       != nullptr);
    log_assert(prev_node      != nullptr);

    unsigned err = LST_OK;

    if (cur_node == fictional_node) { $o return err = err | (1 << LST_INVALID_CYCLE); }

$                                    err = err | list_node_verify(cur_node, false);
    if (cur_node->prev != prev_node) err = err | (1 << LST_INVALID_CYCLE);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_log_error(const list_node *const lst_node, const unsigned err)
{
$i
    if (err == LST_OK) { $o return; }

$   log_error("list_node verify failed\n");

$   for (size_t i = 0; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) { log_tab_error_message("%s", "\n", LST_STATUS_MESSAGES[i]); }
    }

$   list_node_service_fields_dump(lst_node);
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool list_ctor(list *const lst, void (*el_dump) (const void *const) /* = nullptr */)
{
$i
    log_verify(lst != nullptr, false);

    $size    =       0;
    $el_dump = el_dump;

$   if (!list_fictional_ctor(lst)) { $o return false; }

$   list_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

list *list_new(void (*el_dump) (const void *const) /* = nullptr */)
{
$i
$   list *lst = (list *) log_calloc(1, sizeof(list));
    if (lst == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list) = %lu) returns nullptr\n", sizeof(list));
$o      return nullptr;
    }
$   if (!list_ctor(lst, el_dump)) { log_free(lst); $o return nullptr; }

$   list_debug_verify(lst);
$o  return lst;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_fictional_ctor(list *const lst)
{
$i
    log_assert(lst != nullptr);

$   $fictional = (list_node *) log_calloc(1, sizeof(list_node));
    if ($fictional == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list_node) = %lu) returns nullptr\n", sizeof(list_node));
$o      return false;
    }

    $fictional->data =    nullptr;
    $fictional->prev = $fictional;
    $fictional->next = $fictional;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static bool list_node_ctor(list      *const lst,
                           list_node *const lst_node,   const void      *const data,
                                                        const list_node *const prev,
                                                        const list_node *const next)
{
#pragma GCC diagnostic pop
$i
$   list_debug_verify(lst);
    log_assert(lst_node != nullptr);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

    $data =               data;
    $prev = (list_node *) prev;
    $next = (list_node *) next;

#pragma GCC diagnostic pop

$   list_node_debug_verify(lst_node);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_node_new(list *const lst,   const void      *const data,
                                                   const list_node *const prev,
                                                   const list_node *const next)
{
$i
$   list_node *lst_node = (list_node *) log_calloc(1, sizeof(list_node));
    if (lst_node == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list_node)) returns nullptr\n");
$o      return nullptr;
    }

$   if (!list_node_ctor(lst, lst_node, data, prev, next)) { log_free(lst_node); $o return nullptr; }

$   list_node_debug_verify(lst_node);
$o  return lst_node;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void list_dtor(void *const _lst)
{
$i
    if (_lst == nullptr) { $o return; }

    list *const lst = (list *) _lst;
$   list_verify(lst, ;);

$   list_fictional_dtor(lst);
    *lst = LST_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void list_free(void *const _lst)
{
$i
$   list_dtor(_lst);
$   log_free (_lst);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dtor(list *const lst)
{
$i
$   list_debug_verify(lst);
    log_assert($fictional != nullptr);

    list_node *cur_node = $fictional->next;
$   for (; cur_node != $fictional;)
    {
        list_node *next_node = cur_node->next;
        log_free(cur_node);

        cur_node = next_node;
    }
$   log_free($fictional);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_get_node(const list *const lst, const size_t index)
{
$i
$   list_debug_verify(lst);
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

$   list_debug_verify(lst);

$o  return cur_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_insert(list *const lst, const void *const data, const size_t index)
{
$i
$   list_verify(lst,              false);
    log_verify (data  != nullptr, false);
    log_verify (index <=   $size, false);

    list_node *prev_node = nullptr;
    list_node *next_node = nullptr;

    if (index == 0) {   prev_node = $fictional;                    next_node = $fictional->next; }
    else            { $ prev_node = list_get_node(lst, index - 1); next_node = prev_node ->next; }

$   list_node *cur_node = list_node_new(lst, data, prev_node, next_node);
    if (cur_node == nullptr) { $o return false; }

    prev_node->next = cur_node;
    next_node->prev = cur_node;

    $size += 1;

$   list_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_push_front(list *const lst, const void *const data)
{
$i
$   list_verify(lst, false);

$   bool   ret = list_insert(lst, data, 0);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_push_back(list *const lst, const void *const data)
{
$i
$   list_verify(lst, false);

$   bool   ret = list_insert(lst, data, $size);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_erase(list *const lst, const size_t index)
{
$i
$   list_verify(lst,           nullptr);
    log_verify (index < $size, nullptr);

$   list_node  *cur_node  = list_get_node(lst, index);
$   const void *erased_el = cur_node->data;

    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    $size -= 1;

$   log_free(cur_node);

$   list_debug_verify(lst);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

$o  return (void *) erased_el;

#pragma GCC diagnostic pop
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_pop_front(list *const lst)
{
$i
$   void  *erased_el = list_erase(lst, 0);
$o  return erased_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_pop_back(list *const lst)
{
$i
$   list_verify(lst, nullptr);

$   void  *erased_el = list_erase(lst, $size - 1);
$o  return erased_el;
}

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

void *list_get(const list *const lst, const size_t index)
{
$i
$   list_verify(lst,           nullptr);
    log_verify (index < $size, nullptr);

$   const list_node *cur_node = list_get_node(lst, index);
$   const void      *geted_el = cur_node->data;

$   list_debug_verify(lst);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

$o  return (void *) geted_el;

#pragma GCC diagnostic pop
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_front(const list *const lst)
{
$i
$   void  *geted_el = list_get(lst, 0);
$o  return geted_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_back(const list *const lst)
{
$i
$   list_verify(lst, nullptr);

$   void  *geted_el = list_get(lst, $size - 1);
$o  return geted_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_find(const list *const lst, const void *const target, int (*elem_cmp)(const void *elem_1, const void *elem_2))
{
$i
$   list_verify(lst, nullptr);

    log_verify (target   != nullptr, nullptr);
    log_verify (elem_cmp != nullptr, nullptr);

    list_node *lst_node = $fictional->next;
$   while (lst_node != $fictional)
    {
        if (elem_cmp($data, target) == 0) {$o return (void *) $data; }
    }

$o  return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void list_dump(const void *const _lst)
{
$i
    const list *const lst = (const list *) _lst;
$   list_verify(lst, ;);

$   list_static_dump(lst, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_static_dump(const list *const lst, const bool is_full)
{
$i
$   bool are_poison_fields = list_public_fields_dump(lst);
    if (lst == nullptr) { $o return; }

$   list_data_dump(lst, are_poison_fields, is_full);

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_public_fields_dump(const list *const lst)
{
$i
    bool is_any_poison = false;

$   log_tab_message("list (address: %p)\n"
                    "{\n",          lst);
    LOG_TAB++;

$   if (lst == nullptr) { LOG_TAB--; log_tab_message("}\n"); $o return is_any_poison; }

    if      ($el_dump == LST_POISON.el_dump) { $ poison_field_dump ("el_dump  "); is_any_poison = true; }
    else if ($el_dump == nullptr)            { $ warning_field_dump("el_dump  ", "%p",   nullptr);      }
    else                                     { $ usual_field_dump  ("el_dump  ", "%p",  $el_dump);      }

    if      ($size    == LST_POISON.size)    { $ poison_field_dump ("size     "); is_any_poison = true; }
    else                                     { $ usual_field_dump  ("size     ", "%lu",    $size);      }

$o  return is_any_poison;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_dump(const list *const lst, const bool are_poison_fields, const bool is_full)
{
$i
    log_assert(lst != nullptr);

$   if (!list_fictional_dump(lst, are_poison_fields, is_full)) { $o return; }

$   log_tab_message("{\n"); LOG_TAB++;

    const list_node *cur_node = $fictional->next;

    for (size_t i = 0; i < $size; ++i)
    {
        if (cur_node == nullptr)    break;
        if (cur_node == $fictional) break;

$       log_tab_message("#%lu:\n", i);
$       list_node_dump(lst, cur_node, is_full);
        cur_node = cur_node->next;
    }

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_fictional_dump(const list *const lst, bool are_poison_fields, const bool is_full)
{
$i
    log_assert(lst != nullptr);

    if      ($fictional == LST_POISON.fictional) { $ poison_field_dump("fictional"); are_poison_fields = true; }
    else if ($fictional == nullptr)              { $ error_field_dump ("fictional", "%p", $fictional); }
    else                                         { $ usual_field_dump ("fictional", "%p", $fictional); }

    if ($fictional == nullptr) { $o return false; }

    if (are_poison_fields)
    {
$       log_tab_message("{\n"); LOG_TAB++;
$       log_tab_error_message("Some of list's fields are POISON. Can't dump list content", "\n");
$       LOG_TAB--; log_tab_message("}\n");

$o      return false;
    }

$   if (is_full) list_full_fictional_dump(lst);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_full_fictional_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);
    log_assert($fictional != nullptr);
    log_assert($fictional != LST_POISON.fictional);

$   log_tab_message("{" HTML_COLOR_MEDIUM_BLUE "\n"); LOG_TAB++;

    if ($fictional->prev == nullptr) { $ error_field_dump("prev", "%p", $fictional->prev); }
    else                             { $ usual_field_dump("prev", "%p", $fictional->prev); }

    if ($fictional->next == nullptr) { $ error_field_dump("next", "%p", $fictional->next); }
    else                             { $ usual_field_dump("next", "%p", $fictional->next); }

    if ($fictional->data != nullptr) { $ error_field_dump("data", "%p", $fictional->data); }
    else                             { $ good_field_dump ("data", "%p", $fictional->data); }

    LOG_TAB--;
$   log_tab_message(HTML_COLOR_CANCEL "}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_dump(const list *const lst, const list_node *const lst_node, const bool is_full)
{
$i
$   if (is_full) list_node_service_fields_dump(lst_node);

$   log_tab_message("{\n");
    LOG_TAB++;

    if ($data != nullptr)
    {
        if ($el_dump == nullptr) { $ log_tab_warning_message("Can't dump list elements", "\n"); }
        else                     { $ (*$el_dump)($data); }
    }

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_service_fields_dump(const list_node *const lst_node)
{
$i
    log_assert(lst_node != nullptr);

$   log_tab_message(HTML_COLOR_MEDIUM_BLUE
                    "list_node (address: %p)\n"
                    "{\n", lst_node);
    LOG_TAB++;

    if      ($prev == nullptr)        { $ error_field_dump("prev", "%p", $prev); }
    else if ($prev->next != lst_node) { $ error_field_dump("prev", "%p", $prev); }
    else                              { $ usual_field_dump("prev", "%p", $prev); }

    if      ($next == nullptr)        { $ error_field_dump("next", "%p", $next); }
    else if ($next->prev != lst_node) { $ error_field_dump("next", "%p", $next); }
    else                              { $ usual_field_dump("next", "%p", $next); }

    if      ($data == nullptr)        { $ error_field_dump("data", "%p", $data); }
    else                              { $ usual_field_dump("data", "%p", $data); }

    LOG_TAB--;
$   log_tab_message("}" HTML_COLOR_CANCEL "\n");
$o
}
