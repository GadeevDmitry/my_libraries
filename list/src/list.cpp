#include "list_static.h"

//================================================================================================================================

unsigned list_verify(const list *const lst)
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
$   if (err == LST_OK) err = list_cycle_verify(lst);

$   list_log_error(lst, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_log_error(const list *const lst, const unsigned err)
{
$i
    if (err == LST_OK) { $o return; }

$   LOG_ERROR("list verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i))
            LOG_TAB_ERROR_MESSAGE("%s", "\n", LST_STATUS_MESSAGES[i]);
    }

$   LOG_MESSAGE("\n");
$   list_static_dump(lst, true);
$   LOG_TAB_ERROR_MESSAGE(BOLD_LOG_SEP, "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_fields_verify(const list *const lst)
{
$i
    LOG_ASSERT(lst != nullptr);

    unsigned err = LST_OK;

    #define poison_verify(field, status) \
    if (lst->field == LST_POISON.field) err |= (1 << status);

    poison_verify(fictional, LST_POISON_FICTIONAL)
    poison_verify(size     , LST_POISON_SIZE     )
    poison_verify(el_size  , LST_POISON_EL_SIZE  )
    poison_verify(el_dtor  , LST_POISON_EL_DTOR  )
    poison_verify(el_dump  , LST_POISON_EL_DUMP  )

    #undef poison_verify

    if (lst->fictional == nullptr) err |= (1 << LST_NULLPTR_FICTIONAL);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_cycle_verify(const list *const lst)
{
$i
    #define list_node_verify(node)                                      \
    {                                                                   \
        unsigned err = LST_OK;                                          \
        if ((err = list_node_verify(node)) != LST_OK) { $o return err; }\
    }

    LOG_ASSERT(lst            != nullptr);
    LOG_ASSERT(lst->fictional != nullptr);

$   list_node *node_cur  = lst->fictional;       list_node_verify(node_cur );
$   list_node *node_next = lst->fictional->next; list_node_verify(node_next);

    for (size_t i = 0; i < lst->size; ++i)
    {
        if (node_next->prev != node_cur) { $o return 1 << LST_INVALID_CYCLE; }
        if (node_next == lst->fictional) { $o return 1 << LST_INVALID_CYCLE; }

        node_cur  = node_next;
        node_next = node_cur->next;
$       list_node_verify(node_next);
    }

    if (node_next->prev != node_cur) { $o return 1 << LST_INVALID_CYCLE; }
    if (node_next != lst->fictional) { $o return 1 << LST_INVALID_CYCLE; }

$o  return LST_OK;

    #undef list_node_verify
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_node_verify(const list_node *const lst_node)
{
$i
    LOG_ASSERT(lst_node != nullptr);

    if (lst_node->prev == nullptr) { $o return 1 << LST_INVALID_NODE; }
    if (lst_node->next == nullptr) { $o return 1 << LST_INVALID_NODE; }

$o  return LST_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                      void (*el_dump) (const void *const) /* = nullptr */)
{
$i
    LOG_VERIFY(lst != nullptr, false);
    LOG_VERIFY(el_size > 0UL , false);

    lst->size    = 0;
    lst->el_size = el_size;
    lst->el_dtor = el_dtor;
    lst->el_dump = el_dump;

$   if (!list_fictional_ctor(lst)) { $o return false; }

$   LIST_ASSERT(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

list *list_new(const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                     void (*el_dump) (const void *const) /* = nullptr */)
{
$i
$   list *lst = (list *) LOG_CALLOC(1, sizeof(list));
    if (lst == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(list) = %lu) returns nullptr\n", sizeof(list));
$o      return nullptr;
    }
$   if (!list_ctor(lst, el_size, el_dtor, el_dump))
    {
        LOG_FREE(lst);
$o      return nullptr;
    }

$   LIST_ASSERT(lst);
$o  return lst;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_fictional_ctor(list *const lst)
{
$i
    LOG_ASSERT(lst != nullptr);

$   lst->fictional = (list_node *) LOG_CALLOC(1, sizeof(list_node));
    if (lst->fictional == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(list_node) = %lu) returns nullptr\n", sizeof(list_node));
$o      return false;
    }

    lst->fictional->prev = lst->fictional;
    lst->fictional->next = lst->fictional;

$   LIST_ASSERT(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_node_new(list *const lst, const void      *const data,
                                           const list_node *const prev,
                                           const list_node *const next)
{
$i
$   LIST_ASSERT(lst);
    LOG_ASSERT (data != nullptr);
    LOG_ASSERT (prev != nullptr);
    LOG_ASSERT (next != nullptr);

    list_node *lst_node = (list_node *) LOG_CALLOC(1, sizeof(list_node) + lst->el_size);
    if (lst_node == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(list_node) = %lu + el_size = %lu) returns nullptr\n",
                                 sizeof(list_node),   lst->el_size);
$o      return false;
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
$   LIST_ASSERT(lst);
    LOG_ASSERT (lst_node != nullptr);
    LOG_ASSERT (data     != nullptr);
    LOG_ASSERT (prev     != nullptr);
    LOG_ASSERT (next     != nullptr);

    lst_node->prev = (list_node *) prev; lst_node->prev->next = lst_node;
    lst_node->next = (list_node *) next; lst_node->next->prev = lst_node;
$   memcpy(lst_node + 1, data, lst->el_size);

    lst->size++;
$o
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_node_delete(list      *const lst,
                             list_node *const lst_node, void *const erased_data)
{
$i
$   LIST_ASSERT(lst);
    LOG_ASSERT (lst_node != nullptr);

    void *erased_el = lst_node + 1;
    list_node *prev = lst_node->prev;
    list_node *next = lst_node->next;

    prev->next = next;
    next->prev = prev;

    if (erased_data  != nullptr) memcpy(erased_data, erased_el, lst->el_size);
    if (lst->el_dtor != nullptr) { $ lst->el_dtor(erased_el); }
$   LOG_FREE(lst_node);

    lst->size--;

$   LIST_ASSERT(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void list_dtor(void *const _lst)
{
$i
    if (_lst == nullptr) { $o return; }

    list *const lst = (list *) _lst;
$   LIST_VERIFY(lst, (void) 0);

$   list_fictional_dtor(lst);
    *lst = LST_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void list_delete(void *const _lst)
{
$i
$   list_dtor(_lst);
$   LOG_FREE (_lst);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dtor(list *const lst)
{
$i
$   LIST_ASSERT(lst);

    list_node *node_cur  = lst->fictional->next;
    list_node *node_next = node_cur      ->next;

$   for (; node_cur != lst->fictional;)
    {
        if (lst->el_dtor != nullptr) { lst->el_dtor(node_cur + 1); }
        LOG_FREE(node_cur);

        node_cur  = node_next;
        node_next = node_cur->next;
    }

$   LOG_FREE(lst->fictional);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static list_node *list_get_node(const list *const lst, const size_t pos)
{
$i
$   LIST_ASSERT(lst);
    LOG_ASSERT (pos <= lst->size);

    list_node *cur_node = lst->fictional;

    if (2 * pos <= lst->size) { for (size_t i =         0; i <= pos; ++i) cur_node = cur_node->next; }
    else                      { for (size_t i = lst->size; i >  pos; --i) cur_node = cur_node->prev; }

$o  return cur_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

void list_clear(list *const lst)
{
$i
$   LIST_VERIFY(lst, (void) 0);

    size_t initial_size = lst->size;
$   for (size_t i = 0; i < initial_size; ++i)
    {
        list_pop_front(lst);
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_insert(list *const lst, const void *const data, const size_t pos)
{
$i
$   LIST_VERIFY(lst              , false);
    LOG_VERIFY (data != nullptr  , false);
    LOG_VERIFY (pos  <= lst->size, false);

    list_node *prev_node = nullptr;
    list_node *next_node = nullptr;

    if (pos == 0) {   prev_node = lst->fictional;              next_node = lst->fictional->next; }
    else          { $ prev_node = list_get_node(lst, pos - 1); next_node = prev_node     ->next; }

$   bool ret = list_node_new(lst, data, prev_node, next_node);

$   LIST_ASSERT(lst);
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
$   LIST_VERIFY(lst, false);

$   bool ret = list_insert(lst, data, lst->size);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_erase(list *const lst, const size_t pos, void *const erased_data /* = nullptr */)
{
$i
$   LIST_VERIFY(lst, false);
    LOG_VERIFY (pos < lst->size, false);

$   list_node *cur_node = list_get_node(lst, pos);
$   list_node_delete(lst, cur_node, erased_data);

$   LIST_ASSERT(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_front(list *const lst, void *const erased_data /* = nullptr */)
{
$i
$   bool ret = list_erase(lst, 0, erased_data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_back(list *const lst, void *const erased_data /* = nullptr*/)
{
$i
$   LIST_VERIFY(lst, false);

$   bool ret = list_erase(lst, lst->size - 1, erased_data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_get(const list *const lst, const size_t pos)
{
$i
$   LIST_VERIFY(lst            , nullptr);
    LOG_VERIFY (pos < lst->size, nullptr);

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
$   LIST_VERIFY(lst, nullptr);

    if (lst->size == 0) { $o return nullptr; }

$   void *geted_el = list_get(lst, 0);
$o  return geted_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_back(const list *const lst)
{
$i
$   LIST_VERIFY(lst, nullptr);

    if (lst->size == 0) { $o return nullptr; }

$   void *geted_el = list_get(lst, lst->size - 1);
$o  return geted_el;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_fict(const list *const lst)
{
$i
$   LIST_VERIFY(lst, nullptr);

    if (lst->size == 0) { $o return nullptr; }
$o  return lst->fictional + 1;
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

void *list_find(const list *const lst, const void *const target, int (*el_cmp)(const void *el_1, const void *el_2))
{
$i
$   LIST_VERIFY(lst, nullptr);
    LOG_VERIFY (target != nullptr, nullptr);
    LOG_VERIFY (el_cmp != nullptr, nullptr);

$   for (const list_node *cur_node = lst->fictional->next; cur_node != lst->fictional; cur_node = cur_node->next)
    {
        if (el_cmp(cur_node + 1, target) == 0)
        { $o return (void *) (cur_node + 1); }
    }

$o  return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_next(const void *cur_el)
{
$i
    LOG_VERIFY(cur_el != nullptr, nullptr);

    const list_node  *cur_node = (const list_node *) cur_el - 1;
    const list_node *next_node = cur_node->next;

$o  return (void *) (next_node + 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *list_prev(const void *cur_el)
{
$i
    LOG_VERIFY(cur_el != nullptr, nullptr);

    const list_node * cur_node = (const list_node *) cur_el - 1;
    const list_node *prev_node = cur_node->prev;

$o  return (void *) (prev_node + 1);
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

static void list_replace(list_node *src_node, list_node *dest_prev_node)
{
    LOG_ASSERT (src_node       != nullptr);
    LOG_ASSERT (dest_prev_node != nullptr);
    LOG_ASSERT (src_node != dest_prev_node);

    src_node->prev->next = src_node->next;
    src_node->next->prev = src_node->prev;

    src_node->prev = dest_prev_node;
    src_node->next = dest_prev_node->next;

    dest_prev_node->next = src_node;
    src_node->next->prev = src_node;
}

//--------------------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

bool list_replace(const list *const lst, const size_t src_pos, const size_t dest_pos)
{
$i
$   LIST_VERIFY(lst, false);
    LOG_VERIFY (src_pos  < lst->size, false);
    LOG_VERIFY (dest_pos < lst->size, false);

    if (src_pos == dest_pos) { $o return true; }

$   list_node *src_node = list_get_node(lst, src_pos);
$   bool res = list_replace(lst, src_node + 1, src_pos, dest_pos);
$o  return res;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_replace(const list *const lst, const void *src_el, const size_t src_pos, const size_t dest_pos)
{
$i
$   LIST_VERIFY(lst, false);
    LOG_VERIFY (src_el != nullptr, false);
    LOG_VERIFY (src_pos  < lst->size, false);
    LOG_VERIFY (dest_pos < lst->size, false);

    if (src_pos == dest_pos) { $o return true; }

    int dest_prev_pos = ((int) src_pos < (int) dest_pos) ? (int) dest_pos : (int) dest_pos - 1;

    list_node *src_node = (list_node *) src_el - 1;
    LOG_VERIFY(src_node == list_get_node(lst, src_pos), false);

    list_node *dest_prev_node = nullptr;
$   if (dest_prev_pos < 0) dest_prev_node = lst->fictional;
    else                   dest_prev_node = list_get_node(lst, (size_t) dest_prev_pos);

$   list_replace(src_node, dest_prev_node);
$o  return true;
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------

void list_dump(const void *const _lst)
{
$i
    const list *lst = (const list *) _lst;
$   LIST_VERIFY(lst, (void) 0);
$   list_static_dump(lst, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_static_dump(const list *const lst, const bool is_full)
{
$i
$   if (!list_header_dump(lst)) { $o return; }

    bool are_invalid_public_fields =           list_public_fields_dump(lst);
    bool are_invalid_static_fields = is_full ? list_static_fields_dump(lst) : false;
    bool are_invalid_fields        = are_invalid_public_fields || are_invalid_static_fields;

$   list_data_dump(lst, is_full, are_invalid_fields);

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_header_dump(const list *const lst)
{
$i
$   LOG_TAB_SERVICE_MESSAGE("list (address: %p)\n"
                            "{", "\n", lst);

$   if (lst == nullptr)
    {
        LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o      return false;
    }

    LOG_TAB++;
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_public_fields_dump(const list *const lst)
{
$i
    LOG_ASSERT(lst != nullptr);

    bool is_any_invalid = false;

    if      (lst->size    == LST_POISON.size)    { $ POISON_FIELD_DUMP ("size   "); is_any_invalid = true; }
    else                                         { $ USUAL_FIELD_DUMP  ("size   ", "%lu", lst->size); }

    if      (lst->el_size == LST_POISON.el_size) { $ POISON_FIELD_DUMP ("el_size");           is_any_invalid = true; }
    else if (lst->el_size == 0UL)                { $ ERROR_FIELD_DUMP  ("el_size", "%lu", 0); is_any_invalid = true; }
    else                                         { $ USUAL_FIELD_DUMP  ("el_size", "%lu", lst->el_size); }

    if      (lst->el_dtor == LST_POISON.el_dtor) { $ POISON_FIELD_DUMP ("el_dtor"); is_any_invalid = true; }
    else if (lst->el_dtor == nullptr)            { $ WARNING_FIELD_DUMP("el_dtor", "%p",  nullptr); }
    else                                         { $ USUAL_FIELD_DUMP  ("el_dtor", "%p", lst->el_dtor); }

    if      (lst->el_dump == LST_POISON.el_dump) { $ POISON_FIELD_DUMP ("el_dump"); is_any_invalid = true; }
    else if (lst->el_dump == nullptr)            { $ WARNING_FIELD_DUMP("el_dump", "%p",  nullptr); }
    else                                         { $ USUAL_FIELD_DUMP  ("el_dump", "%p", lst->el_dump); }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_static_fields_dump(const list *const lst)
{
$i
    LOG_ASSERT(lst != nullptr);

    bool is_any_invalid = false;

    if      (lst->fictional == LST_POISON.fictional) { $ POISON_FIELD_DUMP("fictional");                is_any_invalid = true; }
    else if (lst->fictional == nullptr)              { $ ERROR_FIELD_DUMP ("fictional", "%p", nullptr); is_any_invalid = true; }
    else                                             { $ USUAL_FIELD_DUMP ("fictional", "%p", lst->fictional); }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_dump(const list *const lst, const bool is_full,
                                                  const bool is_any_invalid)
{
$i
    LOG_ASSERT(lst != nullptr);

$   LOG_TAB_SERVICE_MESSAGE("data\n" "{", "\n");
    LOG_TAB++;

    if (is_any_invalid)
    {
$       LOG_TAB_ERROR_MESSAGE("can't dump it because some of fields are invalid", "\n");
    }
    else if (is_full) { $ list_data_debug_dump (lst); }
    else              { $ list_data_pretty_dump(lst); }

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_debug_dump(const list *const lst)
{
$i
    LOG_ASSERT(lst            != nullptr);
    LOG_ASSERT(lst->fictional != LST_POISON.fictional);
    LOG_ASSERT(lst->fictional != nullptr);

$   LOG_TAB_SERVICE_MESSAGE("#-:\n" "{", "\n");
    LOG_TAB++;
$   list_node_debug_dump(lst, lst->fictional);
    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n");

    const list_node *lst_node = lst->fictional->next;
    for (size_t i = 0; i < lst->size; ++i)
    {
        if (lst_node == lst->fictional) break;

$       LOG_TAB_SERVICE_MESSAGE("#%lu:\n" "{", "\n", i);
        LOG_TAB++;
$       list_node_debug_dump(lst, lst_node);
        LOG_TAB--;
$       LOG_TAB_SERVICE_MESSAGE("}", "\n");

        lst_node = lst_node->next;
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_debug_dump(const list *const lst, const list_node *const lst_node)
{
$i
    LOG_ASSERT(lst_node != nullptr);

$   USUAL_FIELD_DUMP("addr", "%p", lst_node);

    if (lst_node->prev == nullptr) { $ ERROR_FIELD_DUMP("prev", "%p", nullptr); }
    else                           { $ USUAL_FIELD_DUMP("prev", "%p", lst_node->prev); }

    if (lst_node->next == nullptr) { $ ERROR_FIELD_DUMP("next", "%p", nullptr); }
    else                           { $ USUAL_FIELD_DUMP("next", "%p", lst_node->next); }

    if (lst_node == lst->fictional) { $ LOG_TAB_SERVICE_MESSAGE("FICTIONAL", "\n"); $o return; }
$   LOG_MESSAGE("\n");

    if (lst->el_dump == nullptr) { $ LOG_TAB_WARNING_MESSAGE("don't know how to dump the elem", "\n"); }
    else                         { $ lst->el_dump(lst_node + 1); }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_data_pretty_dump(const list *const lst)
{
$i
    LOG_ASSERT(lst            != nullptr);
    LOG_ASSERT(lst->fictional != LST_POISON.fictional);
    LOG_ASSERT(lst->fictional != nullptr);

$   if (lst->el_dump == nullptr) { LOG_TAB_WARNING_MESSAGE("don't know how to dump the content", "\n"); $o return; }

    size_t i = 0;
    for (const list_node *lst_node =  lst->fictional->next;
                          lst_node != lst->fictional; lst_node = lst_node->next)
    {
$       LOG_TAB_SERVICE_MESSAGE("#%lu:\n" "{", "\n", i);
        LOG_TAB++;
$       lst->el_dump(lst_node + 1);
        LOG_TAB--;
$       LOG_TAB_SERVICE_MESSAGE("}", "\n");

        ++i;
    }
$o
}
