#include "stack_static.h"

//================================================================================================================================
// STACK
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

static void stack_gap_fill_poison(stack *const stk)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

    if ($el_poison == nullptr)              { $o return; }
    if ($el_poison == STK_POISON.el_poison) { $o return; }

$  for (size_t i = $size; i < $capacity; ++i) { stack_el_fill_poison(stk, i); }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_fill_poison(stack *const stk, const size_t filled_index)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(filled_index < $capacity);

    log_assert($el_poison !=              nullptr);
    log_assert($el_poison != STK_POISON.el_poison);

$   memcpy(stack_get(stk, filled_index), $el_poison, $el_size);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_gap_is_poison(const stack *const stk)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

    if ($el_poison == nullptr)              { $o return true; }
    if ($el_poison == STK_POISON.el_poison) { $o return true; }

$   for (size_t i = $size; i < $capacity; ++i) { if (!stack_el_is_poison(stk, i)) { $o return false; } }

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_el_is_poison(const stack *const stk, const size_t check_index)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(check_index < $capacity);

    log_assert($el_poison !=              nullptr);
    log_assert($el_poison != STK_POISON.el_poison);

$   bool   ret = is_byte_equal(stack_get(stk, check_index), $el_poison, $el_size);
$o  return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

static void *stack_get(const stack *const stk, const size_t index)
{
$i
    log_assert(stk   !=  nullptr);
    log_assert($data !=  nullptr);
    log_assert(index < $capacity);

$o  return (char *) $data + (index * $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

unsigned _stack_verify(const stack *const stk)
{
$i
    unsigned err = STK_OK;

    if (stk == nullptr)
    {
        err = err | (1 << STK_NULLPTR);
$       stack_log_error(stk, err);
$o      return err;
    }

    if ($data      ==            nullptr  ) err = err | (1 << STK_NULLPTR_DATA          );
    if ($data      == STK_POISON.data     ) err = err | (1 << STK_POISON_DATA           );
    if ($el_size   == STK_POISON.el_size  ) err = err | (1 << STK_POISON_EL_SIZE        );
    if ($size      == STK_POISON.size     ) err = err | (1 << STK_POISON_SIZE           );
    if ($capacity  == STK_POISON.capacity ) err = err | (1 << STK_POISON_CAPACITY       );
    if ($el_poison == STK_POISON.el_poison) err = err | (1 << STK_POISON_EL_POISON      );
    if ($el_dtor   == STK_POISON.el_dtor  ) err = err | (1 << STK_POISON_EL_DTOR        );
    if ($el_dump   == STK_POISON.el_dump  ) err = err | (1 << STK_POISON_EL_DUMP        );
    if ($size      >  $capacity           ) err = err | (1 << STK_INVALID_SIZE_CAPACITY );
$   if (err == STK_OK &&
        !stack_gap_is_poison(stk)         ) err = err | (1 << STK_NOT_POISON_GAP        );

$   stack_log_error(stk, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_log_error(const stack *const stk, const unsigned err)
{
$i
    if (err == STK_OK) { $o return; }

$   log_error("stack verify failed\n");

$   for (size_t i = 0; i * sizeof(char *) < sizeof(STK_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) { log_tab_error_message("%s", "\n", STK_STATUS_MESSAGES[i]); }
    }

$   stack_static_dump(stk, true);
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                              void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                              void (     *el_dump  )(const void *const) /* = nullptr */,

                                                              const size_t stack_capacity /* = DEFAULT_STACK_CAPACITY */)
{
$i
    log_verify(stk      != nullptr, false);
    log_verify(stack_capacity != 0, false);

    $el_size   = el_size;
    $size      = 0;
    $capacity  = stack_capacity;

    $el_poison = el_poison;
    $el_dtor   = el_dtor;
    $el_dump   = el_dump;

$   $data      = log_calloc($el_size, $capacity);
    if ($data == nullptr)
    {
$       log_error("log_calloc(el_size = %lu, stack_capacity = %lu) returns nullptr\n",
                             $el_size      , stack_capacity);
$o      return false;
    }

$   stack_gap_fill_poison(stk);
$   stk_debug_verify     (stk);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

stack *stack_new(const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                             void (     *el_dtor  )(      void *const) /* = nullptr */,
                                             void (     *el_dump  )(const void *const) /* = nullptr */,

                                             const size_t stack_capacity /* = DEFAULT_STACK_CAPACITY */)
{
$i
$   stack *const stk = (stack *) log_calloc(1, sizeof(stack));
    if (stk == nullptr)
    {
$       log_error("log_calloc(1, sizeof(stack) = %lu) returns nullptr\n", sizeof(stack));
$o      return nullptr;
    }
$   if (!stack_ctor(stk, el_size, el_poison, el_dtor, el_dump, stack_capacity)) { log_free(stk); $o return nullptr; }

$   stk_debug_verify(stk);
$o  return stk;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void stack_dtor(void *const _stk)
{
$i
    if (_stk == nullptr) { $o return; }

    stack *const stk = (stack *) _stk;
$   stk_verify(stk, ;);

$   stack_data_dtor(stk);
    *stk = STK_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_free(void *const _stk)
{
$i
$   stack_dtor(_stk);
$   log_free  (_stk);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dtor(stack *const stk)
{
$i
$   stk_debug_verify(stk);

$   if ($el_dtor == nullptr) { log_warning("Can't dtor stack elements\n"); }
    else
    {
$       for (size_t i = 0; i < $size; ++i) { (*$el_dtor)(stack_get(stk, i)); }
    }

$   log_free($data);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_push(stack *const stk, const void *const data)
{
$i
$   stk_verify(stk, false);
    log_verify(data != nullptr, false);

    if ($size == $capacity)
    {
$       if (stack_resize(stk, 2 * $capacity) == false) { $o return false; }
    }

$   memcpy(stack_get(stk, $size), data, $el_size);
    $size++;

$   stk_debug_verify(stk);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_pop(stack *const stk, void *const data /* = nullptr */)
{
$i
$   stk_verify(stk, false);
    log_verify($size != 0, false);

$   if (data != nullptr) memcpy(data, stack_get(stk, $size - 1), $el_size);
    $size--;

$   if ($el_dtor   != nullptr) $el_dtor  (stack_get(stk, $size));
$   if ($el_poison != nullptr) stack_el_fill_poison(stk, $size);

    if (4 * $size <= $capacity)
    {
        size_t new_capacity = 2 * $size;
        if (new_capacity < DEFAULT_STACK_CAPACITY) new_capacity = DEFAULT_STACK_CAPACITY;
$       stack_resize(stk, new_capacity);
    }

$   stk_debug_verify(stk);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_resize(stack *const stk, const size_t new_capacity)
{
$i
$   stk_debug_verify(stk);

    if (new_capacity == $capacity) { $o return true; }

    void *old_data = $data;
$   $data = log_recalloc($data, $capacity * $el_size, new_capacity * $el_size);
    if ($data == nullptr)
    {
$       log_error("log_recalloc(stack.data, (capacity = %lu) * (el_size = %lu), (new_capacity = %lu) * (el_size = %lu)) returns nullptr",
                                            $capacity,         $el_size,         new_capacity,         $el_size);
        $data = old_data;
$o      return false;
    }

    $capacity = new_capacity;
$   stack_gap_fill_poison(stk);
$   stk_debug_verify(stk);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_front(const stack *const stk, void *const data)
{
$i
$   stk_verify(stk, false);
    log_verify(data  != nullptr, false);
    log_verify($size !=       0, false);

$   memcpy(data, stack_get(stk, $size - 1), $el_size);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_is_empty(const stack *const stk)
{
$i
$   stk_verify(stk, false);

$o  return $size == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

void stack_dump(const void *const _stk)
{
$i
    const stack *const stk = (const stack *) _stk;
$   stk_verify(stk, ;);

$   stack_static_dump(stk, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_static_dump(const stack *const stk, const bool is_full)
{
$i
$   stack_public_fields_dump(stk);
    if (stk == nullptr) { $o return; }

    if      ($data == STK_POISON.data) { $ poison_field_dump("data     ");              }
    else if ($data == nullptr)         { $ error_field_dump ("data     ", "%p", $data); }
    else                               { $ stack_data_dump  (stk, is_full);             }

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_public_fields_dump(const stack *const stk)
{
$i
$   log_tab_message("stack (address: %p)\n"
                    "{\n",          stk);
    LOG_TAB++;

$   if (stk == nullptr) { LOG_TAB--; log_tab_message("}\n"); $o return; }

    if      ($el_size   == STK_POISON.el_size  ) { $ poison_field_dump ("el_size  ");                   }
    else                                         { $ usual_field_dump  ("el_size  ", "%lu", $el_size);  }

    if      ($size      == STK_POISON.size     ) { $ poison_field_dump ("size     ");                   }
    else                                         { $ usual_field_dump  ("size     ", "%lu", $size);     }

    if      ($capacity  == STK_POISON.capacity ) { $ poison_field_dump ("capacity ");                   }
    else                                         { $ usual_field_dump  ("capacity ", "%lu", $capacity); }

    if      ($el_poison == STK_POISON.el_poison) { $ poison_field_dump ("el_poison");                   }
    else if ($el_poison == nullptr)              { $ warning_field_dump("el_poison", "%p", $el_poison); }
    else                                         { $ usual_field_dump  ("el_poison", "%p", $el_poison); }

    if      ($el_dtor   == STK_POISON.el_dtor  ) { $ poison_field_dump ("el_dtor  ");                   }
    else if ($el_dtor   == nullptr)              { $ warning_field_dump("el_dtor  ", "%p", $el_dtor);   }
    else                                         { $ usual_field_dump  ("el_dtor  ", "%p", $el_dtor);   }

    if      ($el_dump   == STK_POISON.el_dump  ) { $ poison_field_dump ("el_dump  ");                   }
    else if ($el_dump   == nullptr)              { $ warning_field_dump("el_dump  ", "%p", $el_dump);   }
    else                                         { $ usual_field_dump  ("el_dump  ", "%p", $el_dump);   }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dump(const stack *const stk, const bool is_full)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

$   log_tab_message("data      = %p\n"
                    "{\n",   $data);
    LOG_TAB++;

    for (size_t i = 0; i < $size; ++i)
    {
$       log_tab_message("%lu:\n"
                        "{\n", i);
        LOG_TAB++;
$       stack_el_dump(stk, stack_get(stk, i));
        LOG_TAB--;

$       log_tab_message("}\n");
    }

    if (is_full)
    {
        for (size_t i = $size; i < $capacity; ++i)
        {
$           log_tab_message(HTML_COLOR_MEDIUM_BLUE "%lu:\n"
                            HTML_COLOR_CANCEL      "{\n", i);
            LOG_TAB++;
$           stack_el_dump(stk, stack_get(stk, i));
            LOG_TAB--;

$           log_tab_message("}\n");
        }
    }

    LOG_TAB--;
$   log_tab_message("}\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_dump(const stack *const stk, const void *const el)
{
$i
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(el    != nullptr);

    if ($el_poison != nullptr)
    {
$       if (is_byte_equal($el_poison, el, $el_size))
        {
$           log_tab_poison_message("POISON", "\n");
$o          return;
        }
    }

    if ($el_dump != nullptr) { $ (*$el_dump)(el); }
    else                     { $ log_tab_warning_message("can't dump stack element", "\n"); }
$o
}

//================================================================================================================================
// VECTOR
//================================================================================================================================

/*
*   Далее определны функции структуры данных "vector", которая по сути является стеком (typedef stack vector),
*   но дополнительно предоставляет возможность обращения/изменения любых элементов, а не только верхнего.
*
*   Большинство функций вектора являются inline оболочками для функций стека, поэтому определны в файле "../vector/vector.h",
*   который включен в "../vector/vector_static.h".
*
*   Остальные функции определены в этом файле ниже.
*/

#include "../vector/vector_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// vector iteration
//--------------------------------------------------------------------------------------------------------------------------------

void *vector_get(const vector *const vec, const size_t index)
{
$i
$   vec_verify(vec          , nullptr);
    log_verify(index < $size, nullptr);

$o  return (char *) $data + $el_size * index;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *vector_begin(const vector *const vec)
{
$i
$   vec_verify(vec, nullptr);

$o  return ($size == 0) ? nullptr : $data;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *vector_end(const vector *const vec)
{
$i
$   vec_verify(vec, nullptr);

$o  return ($size == 0) ? nullptr : (char *) $data + $el_size * $size;
}

//--------------------------------------------------------------------------------------------------------------------------------
// vector set
//--------------------------------------------------------------------------------------------------------------------------------

bool vector_set(vector *const vec, const size_t index, const void *const data)
{
$i
$   vec_verify(vec,             false);
    log_verify(index <   $size, false);
    log_verify(data != nullptr, false);

    void  *vec_el = (char *) $data + $el_size * index;
$   memcpy(vec_el, data, $el_size);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// vector resize
//--------------------------------------------------------------------------------------------------------------------------------

bool vector_resize(vector *const vec, const size_t count)
{
$i
$   vec_verify(vec, false);

    if (count == $size) { $o return true; }

    $size = count;
    bool result = true;

    if  (4 * count < $capacity) { $ result = stack_resize(vec, count * 2);  }
    else if (count > $capacity) { $ result = stack_resize(vec, count);      }

$o  return result;
}

//--------------------------------------------------------------------------------------------------------------------------------
// vector dump
//--------------------------------------------------------------------------------------------------------------------------------

void vector_dump(const void *const _vec)
{
$i
    const vector *const vec = (const vector *) _vec;
$   vec_verify(vec, (void) 0);

$   vector_static_dump(vec, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_static_dump(const vector *const vec, const bool is_full)
{
$i
$   if (!vector_header_dump(vec)) { $o return; }

$   bool is_any_public_invalid =           vector_public_fields_dump(vec);
$   bool is_any_static_invalid = is_full ? vector_static_fields_dump(vec) : false;

    bool are_invalid_fields = is_any_public_invalid | is_any_static_invalid;

$   vector_data_dump(vec, is_full, are_invalid_fields);

    LOG_TAB--;
$   log_tab_service_message("}", "\n");

$o  return;
}

//--------------------------------------------------------------------------------------------------------------------------------

static __always_inline bool vector_header_dump(const vector *const vec)
{
$i
$   log_tab_service_message("vector (addr: %p)\n"
                            "{", "\n",    vec);

$   if (vec == nullptr) { log_tab_service_message("{", "\n"); $o return false; }
    LOG_TAB++;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool vector_public_fields_dump(const vector *const vec)
{
$i
    log_assert(vec != nullptr);

    bool is_any_invalid = false;

    if      ($size     == STK_POISON.size)     { $ poison_field_dump("size    ");                   is_any_invalid = true; }
    else if ($size     > $capacity)            { $ error_field_dump ("size    ", "%lu", $size);     is_any_invalid = true; }
    else                                       { $ usual_field_dump ("size    ", "%lu", $size);                            }

    if      ($capacity == STK_POISON.capacity) { $ poison_field_dump("capacity");                   is_any_invalid = true; }
    else if ($capacity <  $size)               { $ error_field_dump ("capacity", "%lu", $capacity); is_any_invalid = true; }
    else                                       { $ usual_field_dump ("capacity", "%lu", $capacity);                        }

    if      ($el_size  == STK_POISON.el_size)  { $ poison_field_dump("el_size ");                   is_any_invalid = true; }
    else                                       { $ usual_field_dump ("el_size ", "%lu", $el_size);                         }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool vector_static_fields_dump(const vector *const vec)
{
$i
    log_assert(vec != nullptr);

    bool is_any_invalid = false;

    if      ($el_dtor   == STK_POISON.el_dtor)   { $ poison_field_dump ("el_dtor  "); is_any_invalid = true; }
    else if ($el_dtor   == nullptr)              { $ warning_field_dump("el_dtor  ", "%p",  nullptr);        }
    else                                         { $ usual_field_dump  ("el_dtor  ", "%p", $el_dtor);        }

    if      ($el_dump   == STK_POISON.el_dump)   { $ poison_field_dump ("el_dump  "); is_any_invalid = true; }
    else if ($el_dump   == nullptr)              { $ warning_field_dump("el_dump  ", "%p",  nullptr);        }
    else                                         { $ usual_field_dump  ("el_dump  ", "%p", $el_dump);        }

    if      ($el_poison == STK_POISON.el_poison) { $ poison_field_dump ("el_poison"); is_any_invalid = true; }
    else if ($el_poison == nullptr)              { $ warning_field_dump("el_poison", "%p",    nullptr);      }
    else                                         { $ usual_field_dump  ("el_poison", "%p", $el_poison);      }

    if      ($data      == STK_POISON.data)      { $ poison_field_dump ("data     ");                is_any_invalid = true; }
    else if ($data      == nullptr)              { $ error_field_dump  ("data     ", "%p", nullptr); is_any_invalid = true; }
    else                                         { $ usual_field_dump  ("data     ", "%p",   $data);         }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_data_dump(const vector *const vec, const bool is_full,
                                                      const bool is_any_invalid)
{
$i
    log_assert(vec != nullptr);

$   log_tab_service_message("data\n"
                            "{", "\n");
    LOG_TAB++;

    if ($data == nullptr || is_any_invalid)
    {
$       log_tab_error_message("can't dump it because some of fields are invalid", "\n");
    }
    else if (is_full) { $ vector_data_static_dump(vec); }
    else              { $ vector_data_public_dump(vec); }

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_data_public_dump(const vector *const vec)
{
$i
    log_assert(vec      != nullptr);
    log_assert($data    != nullptr);
    log_assert($data    != STK_POISON.data);
    log_assert($el_dump != STK_POISON.el_dump);

$   if ($el_dump == nullptr) { log_tab_warning_message("don't know how to dump the content", "\n"); $o return; }

    const char *vec_el = (const char *) $data;

$   for (size_t index = 0; index < $size; ++index)
    {
        log_tab_service_message("#%lu", "\n", index);
        $el_dump(vec_el);

        vec_el += $el_size;
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_data_static_dump(const vector *const vec)
{
$i
    log_assert(vec   != nullptr);
    log_assert($data != nullptr);

    log_assert($data      != STK_POISON.data);
    log_assert($el_dump   != STK_POISON.el_dump);
    log_assert($el_poison != STK_POISON.el_poison);

    if ($el_dump   == nullptr) { $ log_tab_warning_message("don't know how to dump the content", "\n"); }
    else                       { $ vector_data_static_busy_dump(vec); }

    if ($el_poison == nullptr) { $ log_tab_warning_message("don't know POISON-element to verify free elements", "\n"); }
    else                       { $ vector_data_static_free_dump(vec); }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_data_static_busy_dump(const vector *const vec)
{
$i
    log_assert(vec      != nullptr);
    log_assert($data    != nullptr);
    log_assert($data    != STK_POISON.data);
    log_assert($el_dump != nullptr);
    log_assert($el_dump != STK_POISON.el_dump);

    const char *vec_el = (const char *) $data;

$   for (size_t index = 0; index < $size; ++index)
    {
        log_tab_service_message("#%lu (busy)", "\n", index);
        $el_dump(vec_el);

        vec_el += $el_size;
    }
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void vector_data_static_free_dump(const vector *const vec)
{
$i
    log_assert(vec        != nullptr);
    log_assert($data      != nullptr);
    log_assert($el_dump   != nullptr);
    log_assert($el_poison != nullptr);

    log_assert($data      != STK_POISON.data);
    log_assert($el_dump   != STK_POISON.el_dump);
    log_assert($el_poison != STK_POISON.el_poison);

    bool is_any_not_poison = false;

$   for (size_t index = $size; index < $capacity; ++index)
    {
        if (!stack_el_is_poison(vec, index))
        {
            log_tab_service_message("#%lu (free)", "\n", index);
            log_tab_error_message  ("free element is not equal to POISON-element", "\n");

            is_any_not_poison = true;
        }
    }

    if (!is_any_not_poison) log_tab_ok_message("all free elements are equal to POISON-element", "\n");
$o
}
