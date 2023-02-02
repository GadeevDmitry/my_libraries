#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_gap_fill_poison(stack *const stk)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

    if ($el_poison == nullptr)              return;
    if ($el_poison == STK_POISON.el_poison) return;

    for (size_t i = $size; i < $capacity; ++i) { stack_el_fill_poison(stk, i); }
}

static void _stack_gap_fill_poison(const char *const cur_file,
                                   const char *const cur_func,
                                   const int         cur_line,

                                   stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_gap_fill_poison(stk);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_el_fill_poison(stack *const stk, const size_t filled_index)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(filled_index < $capacity);

    if ($el_poison == nullptr)              return;
    if ($el_poison == STK_POISON.el_poison) return;

    memcpy(stack_get(stk, filled_index), $el_poison, $el_size);
}

static void _stack_el_fill_poison(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  stack *const stk, const size_t filled_index)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_el_fill_poison(stk, filled_index);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_gap_is_poison(const stack *const stk)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

    if ($el_poison == nullptr)              return true;
    if ($el_poison == STK_POISON.el_poison) return true;

    for (size_t i = $size; i < $capacity; ++i)
    {
        if (!stack_el_is_poison(stk, i)) return false;
    }
    return true;
}

static bool _stack_gap_is_poison(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_gap_is_poison(stk);
    trace_pop ();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_el_is_poison(const stack *const stk, const size_t check_index)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(check_index < $capacity);

    if ($el_poison == nullptr)              return true;
    if ($el_poison == STK_POISON.el_poison) return true;

    return is_byte_equal(stack_get(stk, check_index), $el_poison, $el_size);
}

static bool _stack_el_is_poison(const char *const cur_file,
                                const char *const cur_func,
                                const int         cur_line,

                                const stack *const stk, const size_t check_index)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_el_is_poison(stk, check_index);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

static void *_stack_get(const stack *const stk, const size_t index)
{
    log_assert(stk   !=  nullptr);
    log_assert($data !=  nullptr);
    log_assert(index < $capacity);

    return (char *) $data + (index * $el_size);
}

static void *_stack_get(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line,

                        const stack *const stk, const size_t index)
{
    trace_push(cur_file, cur_func, cur_line);
    void *ret = _stack_get(stk, index);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _stack_verify(const stack *const stk)
{
    unsigned err = STK_OK;

    if (stk == nullptr)
    {
        err = err | (1 << STK_NULLPTR);
        stack_log_error(stk, err);
        return err;
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
    if (err == STK_OK &&
        !stack_gap_is_poison(stk)         ) err = err | (1 << STK_NOT_POISON_GAP        );

    stack_log_error(stk, err);
    return err;
}

static unsigned _stack_verify(const char *const cur_file,
                              const char *const cur_func,
                              const int         cur_line,

                              const stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    unsigned ret = _stack_verify(stk);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_log_error(const stack *const stk, const unsigned err)
{
    if (err == STK_OK) return;

    log_error("stack verify failed\n");

    for (size_t i = 0; i * sizeof(char *) < sizeof(STK_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_error_message("%s\n", STK_STATUS_MESSAGES[i]);
    }

    stack_static_dump(stk, true);
}

static void _stack_log_error(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const stack *const stk, const unsigned err)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_log_error(stk, err);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                                      void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                                      void (     *el_dump  )(const void *const) /* = nulltpr */)
{
    log_verify(stk != nullptr, false);

    $el_size   = el_size;
    $size      = 0;
    $capacity  = DEFAULT_STACK_CAPACITY;

    $el_poison = el_poison;
    $el_dtor   = el_dtor;
    $el_dump   = el_dump;

    $data      = log_calloc($el_size, $capacity);
    if ($data == nullptr)
    {
        log_error("log_calloc(el_size                = %lu,"
                            " DEFAULT_STACK_CAPACITY = %lu) returns nullptr\n", $el_size, DEFAULT_STACK_CAPACITY);
        return false;
    }

    stack_gap_fill_poison(stk);
    stack_debug_verify   (stk);

    return true;
}

bool _stack_ctor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                               void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                               void (     *el_dump  )(const void *const) /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_ctor(stk, el_size, el_poison, el_dtor, el_dump);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static stack *_stack_new(const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                     void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                     void (     *el_dump  )(const void *const) /* = nullptr */)
{
    stack *const stk = (stack *) log_calloc(1, sizeof(stack));
    if (stk == nullptr)
    {
        log_error("log_calloc(1, sizeof(stack) = %lu) returns nullptr\n", sizeof(stack));
        return nullptr;
    }
    if (!stack_ctor(stk, el_size, el_poison, el_dtor, el_dump)) { log_free(stk); return nullptr; }

    stack_debug_verify(stk);
    return stk;
}

stack *_stack_new(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                              void (     *el_dtor  )(      void *const) /* = nullptr */,
                                              void (     *el_dump  )(const void *const) /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    stack *ret = _stack_new(el_size, el_poison, el_dtor, el_dump);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_dtor(void *const _stk)
{
    stack *const stk = (stack *) _stk;
    stack_verify(stk, ;);

    stack_data_dtor(stk);
    *stk = STK_POISON;
}

void _stack_dtor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 void *const _stk)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_dtor(_stk);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_data_dtor(stack *const stk)
{
    stack_debug_verify(stk);

    if ($el_dtor == nullptr) { log_warning("can't dtor stack elements\n"); }
    else
    {
        for (size_t i = 0; i < $size; ++i) { stack_private_el_dtor($el_dtor, stack_get(stk, i)); }
    }

    log_free($data);
}

static void _stack_data_dtor(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_data_dtor(stk);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_private_el_dtor(const char *const cur_file,
                                   const char *const cur_func,
                                   const int         cur_line,

                                   void (*el_dtor) (void *const),
                                                    void *const el)
{
    trace_push(cur_file, cur_func, cur_line);
    (*el_dtor)(el);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_push(stack *const stk, const void *const data)
{
    stack_verify(stk, false);
    log_verify(data != nullptr, false);

    if ($size == $capacity)
    {
        if (stack_resize(stk, 2 * $capacity) == false) return false;
    }

    memcpy(stack_get(stk, $size), data, $el_size);
    $size++;

    stack_debug_verify(stk);
    return true;
}

bool _stack_push(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_push(stk, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_pop(stack *const stk, void *const data /* = nullptr */)
{
    stack_verify(stk, false);
    log_verify($size != 0, false);

    if (data != nullptr) memcpy(data, stack_get(stk, $size - 1), $el_size);
    $size--;
    stack_el_fill_poison(stk, $size);

    if (4 * $size <= $capacity)
    {
        size_t new_capacity = 2 * $size;
        if (new_capacity < DEFAULT_STACK_CAPACITY) new_capacity = DEFAULT_STACK_CAPACITY;
        stack_resize(stk, new_capacity);
    }

    stack_debug_verify(stk);
    return true;
}

bool _stack_pop(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                stack *const stk, void *const data /* = nullptr */)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_pop(stk, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_resize(stack *const stk, const size_t new_capacity)
{
    stack_debug_verify(stk);

    if (new_capacity == $capacity) return true;

    void *old_data = $data;
    $data = log_realloc($data, new_capacity * $el_size);
    if ($data == nullptr)
    {
        log_error("log_realloc(stack.data,  (new_capacity = %lu) * (el_size = %lu)) returns nullptr",
                                             new_capacity,         $el_size);
        $data = old_data;
        return false;
    }

    $capacity = new_capacity;
    stack_gap_fill_poison(stk);
    stack_debug_verify(stk);

    return true;
}

static bool _stack_resize(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          stack *const stk, const size_t new_capacity)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_resize(stk, new_capacity);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_front(const stack *const stk, void *const data)
{
    stack_verify(stk, false);
    log_verify(data  != nullptr, false);
    log_verify($size !=       0, false);

    memcpy(data, stack_get(stk, $size - 1), $el_size);

    return true;
}

bool _stack_front(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const stack *const stk, void *const data)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_front(stk, data);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_is_empty(const stack *const stk)
{
    stack_verify(stk, false);

    return $size == 0;
}

bool _stack_is_empty(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     const stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    bool ret = _stack_is_empty(stk);
    trace_pop();

    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_dump(const void *const _stk)
{
    const stack *const stk = (const stack *) _stk;
    stack_verify(stk, ;);

    stack_static_dump(stk, false);
}

void _stack_dump(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const void *const _stk)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_dump(_stk);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_static_dump(const stack *const stk, const bool is_full)
{
    stack_public_fields_dump(stk);
    if (stk == nullptr) return;

    if      ($data == STK_POISON.data) poison_field_dump("data     ");
    else if ($data == nullptr)         error_field_dump ("data     ", "%p", $data);
    else                               stack_data_dump  (stk, is_full);

    LOG_TAB--;
    log_tab_message("}\n");
}

static void _stack_static_dump(const char *const cur_file,
                               const char *const cur_func,
                               const int         cur_line,

                               const stack *const stk, const bool is_full)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_static_dump(stk, is_full);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_public_fields_dump(const stack *const stk)
{
    log_tab_message("stack (address: %p)\n"
                    "{\n",          stk);
    LOG_TAB++;

    if (stk == nullptr) { LOG_TAB--; log_tab_message("}\n"); return; }

    if      ($el_size   == STK_POISON.el_size  ) poison_field_dump ("el_size  ");
    else                                         usual_field_dump  ("el_size  ", "%lu", $el_size);

    if      ($size      == STK_POISON.size     ) poison_field_dump ("size     ");
    else                                         usual_field_dump  ("size     ", "%lu", $size);

    if      ($capacity  == STK_POISON.capacity ) poison_field_dump ("capacity ");
    else                                         usual_field_dump  ("capacity ", "%lu", $capacity);

    if      ($el_poison == STK_POISON.el_poison) poison_field_dump ("el_poison");
    else if ($el_poison == nullptr)              warning_field_dump("el_poison", "%p", $el_poison);
    else                                         usual_field_dump  ("el_poison", "%p", $el_poison);

    if      ($el_dtor   == STK_POISON.el_dtor  ) poison_field_dump ("el_dtor  ");
    else if ($el_dtor   == nullptr)              warning_field_dump("el_dtor  ", "%p", $el_dtor);
    else                                         usual_field_dump  ("el_dtor  ", "%p", $el_dtor);

    if      ($el_dump   == STK_POISON.el_dump  ) poison_field_dump ("el_dump  ");
    else if ($el_dump   == nullptr)              warning_field_dump("el_dump  ", "%p", $el_dump);
    else                                         usual_field_dump  ("el_dump  ", "%p", $el_dump);
}

static void _stack_public_fields_dump(const char *const cur_file,
                                      const char *const cur_func,
                                      const int         cur_line,

                                      const stack *const stk)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_public_fields_dump(stk);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_data_dump(const stack *const stk, const bool is_full)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);

    log_tab_message("data      = %p\n"
                    "{\n",   $data);
    LOG_TAB++;

    for (size_t i = 0; i < $size; ++i)
    {
        log_tab_message("%lu:\n"
                        "{\n", i);
        LOG_TAB++;
        stack_el_dump(stk, stack_get(stk, i));
        LOG_TAB--;

        log_tab_message("}\n");
    }

    if (is_full)
    {
        for (size_t i = $size; i < $capacity; ++i)
        {
            log_tab_message(HTML_COLOR_MEDIUM_BLUE "%lu:\n"
                            HTML_COLOR_CANCEL      "{\n", i);
            LOG_TAB++;
            stack_el_dump(stk, stack_get(stk, i));
            LOG_TAB--;

            log_tab_message("}\n");
        }
    }

    LOG_TAB--;
    log_tab_message("}\n");
}

static void _stack_data_dump(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const stack *const stk, const bool is_static)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_data_dump(stk, is_static);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_el_dump(const stack *const stk, const void *const el)
{
    log_assert(stk   != nullptr);
    log_assert($data != nullptr);
    log_assert(el    != nullptr);

    if ($el_poison != nullptr)
    {
        if (is_byte_equal($el_poison, el, $el_size))
        {
            log_tab_message(HTML_COLOR_POISON "POISON" HTML_COLOR_CANCEL "\n");
            return;
        }
    }

    if ($el_dump != nullptr) { stack_private_el_dump($el_dump, el);               }
    else                     { log_warning_message("can't dump stack element\n"); }
}

static void _stack_el_dump(const char *const cur_file,
                           const char *const cur_func,
                           const int         cur_line,

                           const stack *const stk, const void *const el)
{
    trace_push(cur_file, cur_func, cur_line);
    _stack_el_dump(stk, el);
    trace_pop();
}

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_private_el_dump(const char *const cur_file,
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
