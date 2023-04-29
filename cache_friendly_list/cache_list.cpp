#include "cache_list_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// list verify
//--------------------------------------------------------------------------------------------------------------------------------

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

$   err = err | list_poison_verify(lst);
$   err = err | list_fields_verify(lst);

$   if (err != LST_OK) { list_log_error(lst, err); $o return err; }

$   err =       list_free_cycle_verify(lst);
$   err = err | list_busy_cycle_verify(lst);

$   list_log_error(lst, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_log_error(const list *const lst, const unsigned err)
{
$i
    if (err == LST_OK) { $o return; }

$   log_error("cache_list verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(LST_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_error_message("%s", "\n", LST_STATUS_MESSAGES[i]);
    }

$   log_message("\n");

$   list_static_dump(lst, true);

$   log_tab_error_message("====================", "\n");
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_poison_verify(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    unsigned err = LST_OK;

    if ($fictional == LST_POISON.fictional) err = err | (1 << LST_POISON_FICTIONAL);
    if ($el_size   == LST_POISON.el_size  ) err = err | (1 << LST_POISON_EL_SIZE  );
    if ($el_free   == LST_POISON.el_free  ) err = err | (1 << LST_POISON_EL_FREE  );
    if ($size      == LST_POISON.size     ) err = err | (1 << LST_POISON_SIZE     );
    if ($capacity  == LST_POISON.capacity ) err = err | (1 << LST_POISON_CAPACITY );
    if ($el_dtor   == LST_POISON.el_dtor  ) err = err | (1 << LST_POISON_EL_DTOR  );
    if ($el_dump   == LST_POISON.el_dump  ) err = err | (1 << LST_POISON_EL_DUMP  );

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned list_fields_verify(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    unsigned err = LST_OK;

    if ($fictional ==   nullptr) err = err | (1 << LST_NULLPTR_FICTIONAL    );
    if ($size      >= $capacity) err = err | (1 << LST_INVALID_SIZE_CAPACITY);
    if ($el_free   >  $capacity) err = err | (1 << LST_INVALID_EL_FREE      );

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

#define list_free_node_verify(node) if ((err = _list_free_node_verify(lst, node)) != LST_OK) { $o return err; }

static unsigned list_free_cycle_verify(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

    unsigned err = LST_OK;

    const size_t free_cycle_len = $capacity - ($size + 1);
    if (free_cycle_len == 0) { $o return $el_free == $capacity ? LST_OK : LST_INVALID_EL_FREE; }

    list_node *const node_free_first = $fictional + $el_free;

$   list_node *node_cur  = node_free_first;             list_free_node_verify(node_cur );
$   list_node *node_next = $fictional + node_cur->next; list_free_node_verify(node_next);

    for (size_t i = 1; i < free_cycle_len; ++i)
    {
        if (node_next->prev != (size_t) (node_cur - $fictional)) { $o return LST_INVALID_CYCLE; }
        if (node_next == node_free_first)                        { $o return LST_INVALID_CYCLE; }

        node_cur  = node_next;
$       node_next = $fictional + node_cur->next; list_free_node_verify(node_next);
    }

    if (node_next->prev != (size_t) (node_cur - $fictional)) { $o return LST_INVALID_CYCLE; }
    if (node_next != node_free_first)                        { $o return LST_INVALID_CYCLE; }

$o  return LST_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------

#define list_busy_node_verify(node) if ((err = _list_busy_node_verify(lst, node)) != LST_OK) { $o return err; }

static unsigned list_busy_cycle_verify(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

    unsigned err = LST_OK;

    const size_t busy_cycle_len = $size + 1;

$   list_node *node_cur  = $fictional;                  list_free_node_verify(node_cur );
$   list_node *node_next = $fictional + node_cur->next; list_busy_node_verify(node_next);

    for (size_t i = 1; i < busy_cycle_len; ++i)
    {
        if (node_next->prev != (size_t) (node_cur - $fictional)) { $o return LST_INVALID_CYCLE; }
        if (node_next == $fictional)                             { $o return LST_INVALID_CYCLE; }

        node_cur  = node_next;
$       node_next = $fictional + node_cur->next; list_busy_node_verify(node_next);
    }

    if (node_next->prev != (size_t) (node_cur - $fictional)) { $o return LST_INVALID_CYCLE; }
    if (node_next != $fictional)                             { $o return LST_INVALID_CYCLE; }

$o  return LST_OK;
}

#undef list_free_node_verify
#undef list_bust_node_verify

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_free_node_verify(const list *const lst, const list_node *const lst_node)
{
$i
    log_assert(lst      != nullptr);
    log_assert(lst_node != nullptr);

    unsigned err = LST_OK;

    if ($data !=  nullptr) err = err | (1 << LST_FREE_NODE_NOT_NULLPTR_DATA);
    if ($next > $capacity) err = err | (1 << LST_NODE_INVALID_NEXT);
    if ($prev > $capacity) err = err | (1 << LST_NODE_INVALID_PREV);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _list_busy_node_verify(const list *const lst, const list_node *const lst_node)
{
$i
    log_assert(lst      != nullptr);
    log_assert(lst_node != nullptr);

    unsigned err = LST_OK;

    if ($data ==  nullptr) err = err | (1 << LST_BUSY_NODE_NULLPTR_DATA);
    if ($next > $capacity) err = err | (1 << LST_NODE_INVALID_NEXT);
    if ($prev > $capacity) err = err | (1 << LST_NODE_INVALID_PREV);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_ctor(cache_list *const lst, const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                                  void (*el_dump) (const void *const) /* = nullptr */)
{
$i
    log_verify(lst != nullptr, false);

    $el_size  = el_size;
    $el_dtor  = el_dtor;
    $el_dump  = el_dump;

    $capacity = DEFAULT_LIST_CAPACITY;
    $size     = 0;
    $el_free  = 1;

$   if (!list_fictional_ctor(lst)) { $o return false; }

$   list_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

cache_list *cache_list_new(const size_t el_size, void (*el_dtor) (      void *const) /* = nullptr */,
                                                 void (*el_dump) (const void *const) /* = nullptr */)
{
$i
$   list *lst = (list *) log_calloc(1, sizeof(list));
    if (lst == nullptr)
    {
$       log_error("log_calloc(1, sizeof(list) = %lu) returns nullptr\n", sizeof(list));
$o      return nullptr;
    }

$   if (!cache_list_ctor(lst, el_size, el_dtor, el_dump)) { log_free(lst); $o return nullptr; }

$   list_debug_verify(lst);
$o  return lst;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_fictional_ctor(list *const lst)
{
$i
    log_assert(lst != nullptr);

$   $fictional = (list_node *) log_calloc(DEFAULT_LIST_CAPACITY, sizeof(list_node));
    if ($fictional == nullptr)
    {
$       log_error("log_calloc(DEFAULT_LIST_CAPACITY = %lu, sizeof(list_node) = %lu) returns nullptr\n",
                              DEFAULT_LIST_CAPACITY      , sizeof(list_node));
$o      return false;
    }

    $fictional->data = nullptr;
    $fictional->prev = 0;
    $fictional->next = 0;

$   list_free_cycle_ctor(lst);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_free_cycle_ctor(list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

    const size_t free_node_first_ind = $el_free;
    const size_t free_node_last_ind  = $capacity - 1;

$   for (size_t ind = free_node_first_ind + 1; ind < free_node_last_ind; ++ind)
    {
        list_free_node_init(lst, ind, ind - 1, ind + 1);
    }

$   list_free_node_init(lst, free_node_first_ind, free_node_last_ind, size_t_min(free_node_first_ind + 1, free_node_last_ind));
$   list_free_node_init(lst, free_node_last_ind , size_t_max(free_node_last_ind - 1, free_node_first_ind), free_node_first_ind);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_free_node_init(list *const lst, const size_t ind_cur ,
                                                 const size_t ind_prev,
                                                 const size_t ind_next)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

    log_assert(ind_cur  < $capacity);
    log_assert(ind_prev < $capacity);
    log_assert(ind_next < $capacity);

    $fictional[ind_cur].prev = ind_prev;
    $fictional[ind_cur].next = ind_next;
    $fictional[ind_cur].data =  nullptr;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_free_node_new(list *const lst, void *const data, const size_t ind_cur)
{
$i
$   list_debug_verify(lst);

    log_assert(ind_cur < $capacity);
    log_assert(ind_cur > 0);

$   if (data != nullptr) memcpy(data, $fictional[ind_cur].data, $el_size);
$   list_node_dtor(lst, $fictional + ind_cur);

    const size_t ind_prev = $fictional[ind_cur].prev;
    const size_t ind_next = $fictional[ind_cur].next;

    $fictional[ind_prev].next = ind_next;
    $fictional[ind_next].prev = ind_prev;

    if ($el_free == $capacity) { $ list_free_node_init(lst, ind_cur, ind_cur, ind_cur); }
    else                       { $ list_free_node_ctor(lst, ind_cur, $el_free, $fictional[$el_free].prev); }

    $el_free = ind_cur;

$   list_debug_verify(lst);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_free_node_ctor(list *const lst, const size_t ind_cur,
                                                 const size_t ind_next,
                                                 const size_t ind_prev)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);

    log_assert(ind_cur != 0);
    log_assert(ind_cur  < $capacity);
    log_assert(ind_next < $capacity);
    log_assert(ind_prev < $capacity);

    list_node *lst_node = $fictional + ind_cur;

    $data = nullptr;
    $next = ind_next; $fictional[ind_next].prev = ind_cur;
    $prev = ind_prev; $fictional[ind_prev].next = ind_cur;

    $size--;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_busy_node_new(list *const lst, const void *const data, const size_t ind_prev,
                                                                        const size_t ind_next)
{
$i
$   list_debug_verify(lst);
    log_assert(data != nullptr);

    log_assert(ind_prev < $capacity);
    log_assert(ind_next < $capacity);

$   if ($size + 1 == $capacity) { if (!list_resize(lst)) $o return false; }

    const size_t ind_cur = $el_free;

    if ($capacity - $size == 2) $el_free = $capacity;
    else
    {
        size_t free_next = $fictional[$el_free].next;
        size_t free_prev = $fictional[$el_free].prev;

        $fictional[free_next].prev = free_prev;
        $fictional[free_prev].next = free_next;

        $el_free = free_next;
    }

$   bool ret = list_busy_node_ctor(lst, data, ind_cur, ind_prev, ind_next);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_busy_node_ctor(list *const lst, const void *const data, const size_t ind_cur,
                                                                         const size_t ind_prev,
                                                                         const size_t ind_next)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);
    log_assert(data       != nullptr);

    log_assert(ind_cur != 0);
    log_assert(ind_cur  < $capacity);
    log_assert(ind_prev < $capacity);
    log_assert(ind_next < $capacity);

    list_node *lst_node = $fictional + ind_cur;

$   $data = log_calloc(1, $el_size);
    if ($data == nullptr)
    {
$       log_error("log_calloc(1, $el_size = %lu) returns nullptr\n", $el_size);
$o      return false;
    }
$   memcpy($data, data, $el_size);

    $next = ind_next; $fictional[ind_next].prev = ind_cur;
    $prev = ind_prev; $fictional[ind_prev].next = ind_cur;

    $size++;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_resize(list *const lst)
{
$i
$   list_debug_verify(lst);

$   list_node *fictional_new = (list_node *) log_realloc($fictional, 2 * $capacity);
    if (fictional_new == nullptr)
    {
$       log_error("log_realloc($fictional, (2 * $capacity) = %lu) returns nullptr\n", 2 * $capacity);
$o      return false;
    }

    $fictional = fictional_new;
    $capacity *=         2;
    $el_free   = $size + 1;

$   list_free_cycle_ctor(lst);
$   list_debug_verify   (lst);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void cache_list_dtor(void *const _lst)
{
$i
    if (_lst == nullptr) { $o return; }

    list *const lst = (list *) _lst;
$   list_verify(lst, (void) 0);

$   list_fictional_dtor(lst);
    *lst = LST_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void cache_list_free(void *const _lst)
{
$i
$   cache_list_dtor(_lst);
$   log_free       (_lst);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dtor(list *const lst)
{
$i
$   list_debug_verify(lst);

$   if ($el_dtor == nullptr) { log_free($fictional); $o return; }

    list_node *node_cur  = $fictional + $fictional->next;
    list_node *node_next = $fictional + node_cur  ->next;

$   for (; node_cur != $fictional;)
    {
        $el_dtor(node_cur->data);

        node_cur  = node_next;
        node_next = $fictional + node_cur->next;
    }

$   log_free($fictional);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_dtor(list *const lst, list_node *const lst_node)
{
$i
    log_assert(lst      != nullptr);
    log_assert(lst_node != nullptr);

$   if ($el_dtor != nullptr) $el_dtor($data);
$   log_free($data);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

static size_t list_get_node_index(const list *const lst, const size_t pos)
{
$i
$   list_debug_verify(lst);
    log_assert(pos <= $size);

    size_t cur_index = 0;

    if (pos <= $size / 2) { for (size_t i =     0; i <= pos; ++i) cur_index = $fictional[cur_index].next; }
    else                  { for (size_t i = $size; i >  pos; --i) cur_index = $fictional[cur_index].prev; }

$   list_debug_verify(lst);
$o  return cur_index;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_insert(cache_list *const lst, const void *const data, const size_t pos)
{
$i
$   list_verify(lst,             false);
    log_verify (data != nullptr, false);
    log_verify (pos  <=   $size, false);

    size_t ind_prev = 0;
    size_t ind_next = 0;

    if (pos == 0) { $ ind_prev = 0;                                 ind_next = $fictional->next;          }
    else          { $ ind_prev = list_get_node_index(lst, pos - 1); ind_next = $fictional[ind_prev].next; }

$   list_busy_node_new(lst, data, ind_prev, ind_next);

$   list_debug_verify(lst);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_push_front(cache_list *const lst, const void *const data)
{
$i
$   bool   ret = cache_list_insert(lst, data, 0);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_push_back(cache_list *const lst, const void *const data)
{
$i
$   bool   ret = cache_list_insert(lst, data, $size);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_erase(cache_list *const lst, const size_t pos, void *const data /* = nullptr */)
{
$i
$   list_verify(lst,         false);
    log_verify (pos < $size, false);

$   size_t ind_cur = list_get_node_index(lst, pos);
$   list_free_node_new(lst, data, ind_cur);

$   list_debug_verify(lst);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_front(cache_list *const lst, void *const data /* = nullptr */)
{
$i
$   bool   ret = cache_list_erase(lst, 0, data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool list_pop_back(cache_list *const lst, void *const data /* = nullptr */)
{
$i
$   bool   ret = cache_list_erase(lst, $size - 1, data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_get(const cache_list *const lst, const size_t pos, void *const data)
{
$i
$   list_verify(lst,             false);
    log_verify (pos   <   $size, false);
    log_verify (data != nullptr, false);

$   const list_node *lst_node = $fictional + list_get_node_index(lst, pos);
$   memcpy(data, $data, $el_size);

$   list_debug_verify(lst);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_front(const cache_list *const lst, void *const data)
{
$i
$   bool   ret = cache_list_get(lst, 0, data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool cache_list_back(const cache_list *const lst, void *const data)
{
$i
$   bool   ret = cache_list_get(lst, $size - 1, data);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void list_dump(const void *const _lst)
{
$i
    const list *const lst = (const list *) _lst;
$   list_verify(lst, (void) 0);

$   list_static_dump(lst, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_static_dump(const list *const lst, const bool is_full)
{
$i
$   if (!list_header_dump(lst)) { $o return; }

$   bool are_invalid_public_fields =           list_public_fields_dump(lst);
$   bool are_invalid_static_fields = is_full ? list_static_fields_dump(lst) : false;

    bool are_invalid_fields = are_invalid_public_fields | are_invalid_static_fields;

$   list_fictional_dump(lst, is_full, are_invalid_fields);

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static __always_inline bool list_header_dump(const list *const lst)
{
$i
$   log_tab_service_message("cache_list (address: %p)\n"
                            "{", "\n",            lst);

$   if (lst == nullptr) { log_tab_service_message("}", "\n"); $o return false; }
    LOG_TAB++;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_public_fields_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    bool is_any_invalid = false;

    if      ($el_size == LST_POISON.el_size) { $ poison_field_dump ("el_size"); is_any_invalid = true; }
    else                                     { $ usual_field_dump  ("el_size", "%lu", $el_size);       }

    if      ($el_dtor == LST_POISON.el_dtor) { $ poison_field_dump ("el_dtor"); is_any_invalid = true; }
    else if ($el_dtor == nullptr)            { $ warning_field_dump("el_dtor", "%p",   nullptr);       }
    else                                     { $ usual_field_dump  ("el_dtor", "%p",  $el_dtor);       }

    if      ($el_dump == LST_POISON.el_dump) { $ poison_field_dump ("el_dump"); is_any_invalid = true; }
    else if ($el_dump == nullptr)            { $ warning_field_dump("el_dump", "%p",   nullptr);       }
    else                                     { $ usual_field_dump  ("el_dump", "%p",  $el_dump);       }

    if      ($size    == LST_POISON.size)    { $ poison_field_dump ("size   ");               is_any_invalid = true; }
    else if ($size    >  $capacity)          { $ error_field_dump  ("size   ", "%lu", $size); is_any_invalid = true; }
    else                                     { $ usual_field_dump  ("size   ", "%lu", $size);          }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool list_static_fields_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);

    bool is_any_invalid = false;

    if      ($fictional == LST_POISON.fictional) { $ poison_field_dump("fictional"); is_any_invalid = true; }
    else if ($fictional == nullptr)              { $ error_field_dump ("fictional", "%p",    nullptr);      }
    else                                         { $ usual_field_dump ("fictional", "%p", $fictional);      }

    if      ($el_free   == LST_POISON.el_free)   { $ poison_field_dump("el_free  ");                  is_any_invalid = true; }
    else if ($el_free   >  $capacity)            { $ error_field_dump ("el_free  ", "%lu", $el_free); is_any_invalid = true; }
    else                                         { $ usual_field_dump ("el_free  ", "%lu", $el_free);       }

    if      ($capacity  == LST_POISON.capacity)  { $ poison_field_dump("capacity "); is_any_invalid = true; }
    else if ($capacity  <  $size)                { $ error_field_dump ("capacity ", "%lu", $capacity);      }
    else                                         { $ usual_field_dump ("capacity ", "%lu", $capacity);      }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_dump(const list *const lst, const bool is_full,
                                                       const bool is_any_invalid)
{
$i
    log_assert(lst != nullptr);

$   log_tab_service_message("fictional\n"
                            "{", "\n");
    LOG_TAB++;

    if ($fictional == nullptr || is_any_invalid)
    {
$       log_tab_error_message("can't dump it because some of fields are invalid", "\n");
    }
    else if (is_full) { $ list_fictional_static_dump(lst); }
    else              { $ list_fictional_public_dump(lst); }

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_static_dump(const list *const lst)
{
$i
    log_assert(lst        != nullptr);
    log_assert($fictional != nullptr);
    log_assert($fictional != LST_POISON.fictional);

    for (size_t ind = 0; ind < $capacity; ++ind)
    {
$       log_service_message("#%lu\n{", "\n", ind); LOG_TAB++;
$       list_node_static_dump(lst, $fictional + ind);
$       LOG_TAB--; log_service_message("}", "\n");
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_node_static_dump(const list *const lst, const list_node *const lst_node)
{
$i
    log_assert(lst_node != nullptr);

    if ($prev >= $capacity) { $ error_field_dump  ("prev", "%lu", $prev);  }
    else                    { $ usual_field_dump  ("prev", "%lu", $prev);  }

    if ($next >= $capacity) { $ error_field_dump  ("next", "%lu", $next);  }
    else                    { $ usual_field_dump  ("next", "%lu", $next);  }

    if ($data == nullptr)   { $ warning_field_dump("data", "%p", nullptr); }
    else                    { $ usual_field_dump  ("data", "%p", nullptr); }

    if ($data != nullptr)
    {
$       log_message("\n");
        if ($el_dump == nullptr) { $ log_tab_warning_message("don't know how to dump \"data\" field", "\n"); }
        else                     { $ $el_dump($data); }
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void list_fictional_public_dump(const list *const lst)
{
$i
    log_assert(lst != nullptr);
    log_assert($fictional != nullptr);
    log_assert($fictional != LST_POISON.fictional);

$   if ($el_dump == nullptr) { log_tab_warning_message("don't know how to dump the content", "\n"); $o return; }

    size_t pos = 0;

    for (size_t ind = $fictional->next; ind != 0; ind = $fictional[ind].next)
    {
$       log_service_message("#%lu\n", "\n", pos); pos++;
$       $el_dump($fictional[ind].data);
    }
$o
}
