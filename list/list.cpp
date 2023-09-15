#include "list_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

unsigned _list_verify(const list *const lst)
{
$i
    unsigned err = LST_OK;

    if (lst == nullptr)
    {
        err = (1 << LST_NULLPTR);
$       list_log_error(lst, err);
$o      return err;
    }

$   err = list_fields_verify(lst);
$   if (err != LST_OK) { list_log_error(lst, err); $o return err; }
$   err = list_cycle_verify(lst);

$   list_log_error(lst, err);
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

$   log_message("\n");

$   list_static_dump(lst, true);

$   log_tab_error_message("====================", "\n");
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_fields_verify(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    unsigned err = LST_OK;

    #define poison_verify(lst_field, err_field) \
    if (lst->lst_field == LST_POISON.lst_field) err = err | (1 << err_field);

    if ($fictional     == nullptr             ) err = err | (1 << LST_NULLPTR_FICTIONAL);

    poison_verify(fictional, LST_POISON_FICTIONAL)
    poison_verify(size     , LST_POISON_SIZE     )
    poison_verify(el_size  , LST_POISON_EL_SIZE  )
    poison_verify(el_dtor  , LST_POISON_EL_DTOR  )
    poison_verify(el_dump  , LST_POISON_EL_DUMP  )

    #undef poison_verify

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

#define list_node_verify(node)                                                  \
{                                                                               \
    unsigned err = LST_OK;                                                      \
    if ((err = _list_node_verify(node)) != LST_OK) { $o return err; }           \
}

static unsigned list_cycle_verify(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

$   list_node *node_cur  = $fictional;       list_node_verify(node_cur );
$   list_node *node_next = $fictional->next; list_node_verify(node_next);

    for (size_t i = 0; i < $size; ++i)
    {
        if (node_next->prev != node_cur) { $o return 1 << LST_INVALID_CYCLE; }
        if (node_next == $fictional)     { $o return 1 << LST_INVALID_CYCLE; }

        node_cur  = node_next;
        node_next = node_cur->next;
$       list_node_verify(node_next);
    }

    if (node_next->prev != node_cur) { $o return 1 << LST_INVALID_CYCLE; }
    if (node_next != $fictional)     { $o return 1 << LST_INVALID_CYCLE; }

$o  return LST_OK;
}

#undef list_node_verify

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_node_verify(const list_node *const lst_node)
{
$i
    log_assert(lst_node != nullptr);

    if ($prev == nullptr) { $o return 1 << LST_INVALID_NODE; }
    if ($next == nullptr) { $o return 1 << LST_INVALID_NODE; }

$o  return LST_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                      void (*el_dump) (const void *const) /* = nullptr */)
{
$i
    log_verify(lst != nullptr, false);
    log_verify(el_size > 0UL , false);

    $size    =       0;
    $el_size = el_size;
    $el_dtor = el_dtor;
    $el_dump = el_dump;

$   if (!list_fictional_ctor(lst)) { $o return false; }

$   lst_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

list *list_new(const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                     void (*el_dump) (const void *const) /* = nullptr */)
{
$i
$   list *lst = (list *) log_calloc(1, sizeof(list));
    if (lst == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list) = %lu) returns nullptr\n", sizeof(list));
$o      return nullptr;
    }
$   if (!list_ctor(lst, el_size, el_dtor, el_dump)) { log_free(lst); $o return nullptr; }

$   lst_debug_verify(lst);
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

    $fictional->prev = $fictional;
    $fictional->next = $fictional;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_node_new(list *const lst, const void      *const data,
                                           const list_node *const prev,
                                           const list_node *const next)
{
$i
$   lst_debug_verify(lst);
    log_assert(data != nullptr);
    log_assert(prev != nullptr);
    log_assert(next != nullptr);

    list_node *lst_node = (list_node *) log_calloc(1, sizeof(list_node) + $el_size);
    if (lst_node == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list_node) = %lu + el_size = %lu) returns nullptr\n",
                                 sizeof(list_node),       $el_size);   $o return false;
    }

$   list_node_ctor(lst, lst_node, data, prev, next);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

static void list_node_ctor(list      *const lst,
                           list_node *const lst_node, const void      *const data,
                                                      const list_node *const prev,
                                                      const list_node *const next)
{
$i
    lst_debug_verify(lst);

    log_assert(lst_node != nullptr);
    log_assert(data     != nullptr);
    log_assert(prev     != nullptr);
    log_assert(next     != nullptr);

    $prev = (list_node *) prev; $prev->next = lst_node;
    $next = (list_node *) next; $next->prev = lst_node;
$   memcpy(lst_node + 1, data, $el_size);

    $size++;
$o
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_node_delete(list      *const lst,
                             list_node *const lst_node, void *const erased_data)
{
$i
$   lst_debug_verify(lst);
    log_assert(lst_node != nullptr);

    void *erased_el = lst_node + 1;
    list_node *prev = lst_node->prev;
    list_node *next = lst_node->next;

    prev->next = next;
    next->prev = prev;

    if (erased_data != nullptr) memcpy(erased_data, erased_el, $el_size);
    if ($el_dtor    != nullptr) { $ $el_dtor(erased_el); }
$   log_free(lst_node);

    $size--;

$   lst_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void list_dtor(void *const _lst)
{
$i
    if (_lst == nullptr) { $o return; }

    list *const lst = (list *) _lst;
$   lst_verify(lst, (void) 0);

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
$   lst_debug_verify(lst);

    list_node *node_cur  = $fictional->next;
    list_node *node_next = node_cur  ->next;

$   for (; node_cur != $fictional;)
    {
        if ($el_dtor != nullptr) { $el_dtor(node_cur + 1); }
        log_free(node_cur);

        node_cur  = node_next;
        node_next = node_cur->next;
    }

$   log_free($fictional);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_get_node(const list *const lst, const size_t pos)
{
$i
$   lst_debug_verify(lst);
    log_assert(pos <= $size);

    list_node *cur_node = $fictional;

    if (2 * pos <= $size) { for (size_t i =     0; i <= pos; ++i) cur_node = cur_node->next; }
    else                  { for (size_t i = $size; i >  pos; --i) cur_node = cur_node->prev; }

$o  return cur_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_insert(list *const lst, const void *const data, const size_t pos)
{
$i
$   lst_verify(lst            , false);
    log_verify(data != nullptr, false);
    log_verify(pos  <=   $size, false);

    list_node *prev_node = nullptr;
    list_node *next_node = nullptr;

    if (pos == 0) {   prev_node = $fictional;                  next_node = $fictional->next; }
    else          { $ prev_node = list_get_node(lst, pos - 1); next_node = prev_node ->next; }

$   bool ret = list_node_new(lst, data, prev_node, next_node);

$   lst_debug_verify(lst);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_push_front(list *const lst, const void *const data)
{
$i
$   bool   ret = list_insert(lst, data, 0);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_push_back(list *const lst, const void *const data)
{
$i
$   lst_verify(lst, false);

$   bool   ret = list_insert(lst, data, $size);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_erase(list *const lst, const size_t pos, void *const erased_data /* = nullptr */)
{
$i
$   lst_verify(lst,         false);
    log_verify(pos < $size, false);

$   list_node *cur_node = list_get_node(lst, pos);
$   list_node_delete(lst, cur_node, erased_data);

$   lst_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_erase(list *const lst, const void *const target, int (*el_cmp)(const void *el_1, const void *el_2),
                void *const erased_data /* = nullptr */)
{
$i
$   lst_verify(lst,               false);
    log_verify(target != nullptr, false);
    log_verify(el_cmp != nullptr, false);

$   size_t cur_pos = list_find_pos(lst, target, el_cmp);
    if (cur_pos == -1UL) { $o return false; }

$   bool result = list_erase(lst, cur_pos, erased_data);
$o  return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_front(list *const lst, void *const erased_data /* = nullptr */)
{
$i
$   bool   ret = list_erase(lst, 0, erased_data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_back(list *const lst, void *const erased_data /* = nullptr*/)
{
$i
$   lst_verify(lst, false);

$   bool   ret = list_erase(lst, $size - 1, erased_data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

void *list_get(const list *const lst, const size_t pos)
{
$i
$   lst_verify(lst        , nullptr);
    log_verify(pos < $size, nullptr);

$   const list_node *cur_node = list_get_node(lst, pos);
$   const void      *geted_el = cur_node + 1;

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
$   lst_verify(lst, nullptr);

$   void  *geted_el = list_get(lst, $size - 1);
$o  return geted_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

void *list_find(const list *const lst, const void *const target, int (*el_cmp)(const void *el_1, const void *el_2))
{
$i
$   lst_verify(lst, nullptr);
    log_verify(target != nullptr, nullptr);
    log_verify(el_cmp != nullptr, nullptr);

$   for (const list_node *cur_node = $fictional->next; cur_node != $fictional; cur_node = cur_node->next)
        if (el_cmp(cur_node + 1, target) == 0) { $o return (void *) (cur_node + 1); }

$o  return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------

static size_t list_find_pos(const list *const lst, const void *const target, int (*el_cmp)(const void *el_1, const void *el_2))
{
$i
$   lst_debug_verify(lst);
    log_assert(target != nullptr);
    log_assert(el_cmp != nullptr);

    size_t pos = 0;

$   for (const list_node *cur_node = $fictional->next; cur_node != $fictional; cur_node = cur_node->next)
    {
        if (el_cmp(cur_node + 1, target) == 0) { $o return pos; }
        ++pos;
    }
$o  return -1UL;
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void list_dump(const void *const _lst)
{
$i
    const list *const lst = (const list *) _lst;
$   lst_verify(lst, (void) 0);

    #if !defined(NDEBUG) && !defined(LIST_NDEBUG)
$   list_static_dump(lst, true);
    #else
$   list_static_dump(lst, false);
    #endif
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_static_dump(const list *const lst, const bool is_full)
{
$i
$   if (!list_header_dump(lst)) { $o return; }

    bool are_invalid_public_fields =           list_public_fields_dump(lst);
    bool are_invalid_static_fields = is_full ? list_static_fields_dump(lst) : false;

    bool are_invalid_fields = are_invalid_public_fields | are_invalid_static_fields;

$   list_data_dump(lst, is_full, are_invalid_fields);

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_header_dump(const list *const lst)
{
$i
$   log_tab_service_message("list (address: %p)\n"
                            "{", "\n",     lst);

$   if (lst == nullptr) { log_tab_service_message("}", "\n"); $o return false; }
    LOG_TAB++;;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_public_fields_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    bool is_any_invalid = false;

    if      ($size    == LST_POISON.size)    { $ poison_field_dump ("size   "); is_any_invalid = true; }
    else                                     { $ usual_field_dump  ("size   ", "%lu", $size); }

    if      ($el_size == LST_POISON.el_size) { $ poison_field_dump ("el_size");           is_any_invalid = true; }
    else if ($el_size == 0UL)                { $ error_field_dump  ("el_size", "%lu", 0); is_any_invalid = true; }
    else                                     { $ usual_field_dump  ("el_size", "%lu", $el_size); }

    if      ($el_dtor == LST_POISON.el_dtor) { $ poison_field_dump ("el_dtor"); is_any_invalid = true; }
    else if ($el_dtor == nullptr)            { $ warning_field_dump("el_dtor", "%p",  nullptr); }
    else                                     { $ usual_field_dump  ("el_dtor", "%p", $el_dtor); }

    if      ($el_dump == LST_POISON.el_dump) { $ poison_field_dump ("el_dump"); is_any_invalid = true; }
    else if ($el_dump == nullptr)            { $ warning_field_dump("el_dump", "%p",  nullptr); }
    else                                     { $ usual_field_dump  ("el_dump", "%p", $el_dump); }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_static_fields_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    bool is_any_invalid = false;

    if      ($fictional == LST_POISON.fictional) { $ poison_field_dump("fictional");                is_any_invalid = true; }
    else if ($fictional == nullptr)              { $ error_field_dump ("fictional", "%p", nullptr); is_any_invalid = true; }
    else                                         { $ usual_field_dump ("fictional", "%p", $fictional); }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_dump(const list *const lst, const bool is_full,
                                                  const bool is_any_invalid)
{
$i
    log_assert(lst != nullptr);

$   log_tab_service_message("data\n"
                            "{", "\n");
    LOG_TAB++;

    if (is_any_invalid)
    {
$       log_tab_error_message("can't dump it because some of fields are invalid", "\n");
    }
    else if (is_full) { $ list_data_debug_dump (lst); }
    else              { $ list_data_pretty_dump(lst); }

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_debug_dump(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != LST_POISON.fictional);
    log_assert($fictional != nullptr);

    const list_node *lst_node = $fictional;

    for (size_t cur_ind = 0; cur_ind <= $size; ++cur_ind)
    {
$       log_tab_service_message("#%lu\n{", "\n", cur_ind);
        LOG_TAB++;

$       list_node_debug_dump(lst, lst_node);

        LOG_TAB--;
$       log_tab_service_message("}", "\n");

        lst_node = $next;
        if (lst_node == $fictional) break;
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_debug_dump(const list *const lst, const list_node *const lst_node)
{
$i
    log_assert(lst_node != nullptr);

$   usual_field_dump("addr", "%p", lst_node);

    if ($prev == nullptr) { $ error_field_dump("prev", "%p", nullptr); }
    else                  { $ usual_field_dump("prev", "%p", $prev  ); }

    if ($next == nullptr) { $ error_field_dump("next", "%p", nullptr); }
    else                  { $ usual_field_dump("next", "%p", $next  ); }

    if (lst_node == $fictional) { $ log_tab_service_message("FICTIONAL", "\n"); $o return; }
$   log_message("\n");

    if ($el_dump == nullptr) { $ log_tab_warning_message("don't know how to dump the elem", "\n"); }
    else                     { $ $el_dump(lst_node + 1); }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_pretty_dump(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != LST_POISON.fictional);
    log_assert($fictional != nullptr);

$   if ($el_dump == nullptr) { log_tab_warning_message("don't know how to dump the content", "\n"); $o return; }

    size_t cur_pos = 0;
    for (const list_node *lst_node = $fictional->next; lst_node != $fictional; lst_node = $next)
    {
$       log_tab_service_message("#%lu\n{", "\n", cur_pos);
        LOG_TAB++;

$       $el_dump(lst_node + 1);
        cur_pos++;

        LOG_TAB--;
$       log_tab_service_message("}", "\n");
    }
$o
}
