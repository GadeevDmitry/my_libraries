#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack_static.h"

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STATIC FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

static void stack_gap_fill_poison(stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    if ($el_poison == nullptr)              return;
    if ($el_poison == STK_POISON.el_poison) return;

    for (size_t i = $size; i < $capacity; ++i) { stack_el_fill_poison(stk, i); }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_fill_poison(stack *const stk, const size_t filled_index)
{
    assert(stk != nullptr);
    assert($data != nullptr);
    assert(filled_index < $capacity);

    if ($el_poison == nullptr)              return;
    if ($el_poison == STK_POISON.el_poison) return;

    memcpy(stack_get(stk, filled_index), $el_poison, $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_gap_is_poison(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    if ($el_poison == nullptr)              return true;
    if ($el_poison == STK_POISON.el_poison) return true;

    for (size_t i = $size; i < $capacity; ++i)
    {
        if (!stack_el_is_poison(stk, i)) return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_el_is_poison(const stack *const stk, const size_t check_index)
{
    assert(stk   != nullptr);
    assert($data != nullptr);
    assert(check_index < $capacity);

    if ($el_poison == nullptr)              return true;
    if ($el_poison == STK_POISON.el_poison) return true;

    return is_byte_equal(stack_get(stk, check_index), $el_poison, $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

static void *stack_get(const stack *const stk, const size_t index)
{
    assert(stk   !=  nullptr);
    assert($data !=  nullptr);
    assert(index < $capacity);

    return (char *) $data + (index * $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned stack_verify(const stack *const stk,    const char *const file,
                                                        const char *const func,
                                                        const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    unsigned err = STK_OK;

    if (stk == nullptr)
    {
        err = err | (1 << STK_NULLPTR);
        stack_log_error(stk, err, file, func, line);
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

    stack_log_error(stk, err, file, func, line);
    return err;
}

#define stack_verify(stk) stack_verify(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#ifdef STACK_DEBUG
#define stack_debug_verify(stk)                                 \
        if (stack_verify(stk) != STK_OK)                        \
        {                                                       \
            log_assert(false && "stack_debug_verify failed");   \
        }
#else
#define stack_debug_verify(stk) ;
#endif

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_log_error(const stack *const stk, const unsigned err, const char *const file,
                                                                        const char *const func,
                                                                        const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    if (err == STK_OK) return;

    log_param_error(file, func, line, "stack verify failed\n");
    log_message    (HTML_COLOR_DARK_RED);

    for (size_t i = 0; i * sizeof(char *) < sizeof(STK_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_message("%s\n", STK_STATUS_MESSAGES[i]);
    }

    log_message(HTML_COLOR_CANCEL);
    stack_static_dump(stk, file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack static dump
//--------------------------------------------------------------------------------------------------------------------------------

#define poison_field_dump(field)                         log_tab_message(field " = " HTML_COLOR_POISON      "POISON"       HTML_COLOR_CANCEL "\n")

#define good_field_dump(   field, specified_char, value) log_tab_message(field " = " HTML_COLOR_LIME_GREEN  specified_char HTML_COLOR_CANCEL "\n", value)
#define warning_field_dump(field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_ORANGE specified_char HTML_COLOR_CANCEL "\n", value)
#define error_field_dump(  field, specified_char, value) log_tab_message(field " = " HTML_COLOR_DARK_RED    specified_char HTML_COLOR_CANCEL "\n", value)
#define usual_field_dump(  field, specified_char, value) log_tab_message(field " = "                        specified_char                   "\n", value)

static void stack_static_dump(const stack *const stk,   const char *const file,
                                                        const char *const func,
                                                        const int         line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_tab_message("stack_dump called by error in\n"
                    "    FILE: %s\n"
                    "FUNCTION: %s\n"
                    "    LINE: %d\n", file, func, line);

    stack_dump(stk, true);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_dump(const stack *const stk, const bool is_static)
{
    stack_public_fields_dump(stk);
    if (stk == nullptr) return;

    if      ($data == STK_POISON.data) poison_field_dump("data     ");
    else if ($data == nullptr)         error_field_dump ("data     ", "%p", $data);
    else                               stack_data_dump  (stk, is_static);

    LOG_TAB--;
    log_tab_message("}\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_public_fields_dump(const stack *const stk)
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

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dump(const stack *const stk, const bool is_static)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

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

    if (is_static)
    {
        for (size_t i = $size; i < $capacity; ++i)
        {
            log_tab_message(HTML_COLOR_MEDIUM_BLUE "%lu:\n",
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

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_dump(const stack *const stk, const void *const el)
{
    assert(stk   != nullptr);
    assert($data != nullptr);
    assert(el    != nullptr);

    if ($el_poison != nullptr)
    {
        if (is_byte_equal($el_poison, el, $el_size))
        {
            log_tab_message(HTML_COLOR_POISON "POISON" HTML_COLOR_CANCEL "\n");
            return;
        }
    }

    if ($el_dump != nullptr) { (*$el_dump)(el);                           }
    else                     { log_warning("can't dump stack element\n"); }
}

//================================================================================================================================
// USER FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                              void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                              void (     *el_dump  )(const void *const) /* = nulltpr */)
{
    if (stk == nullptr) { stack_log_error(stk, STK_NULLPTR, __FILE__, __PRETTY_FUNCTION__, __LINE__); return false; }

    $el_size   = el_size;
    $size      = 0;
    $capacity  = DEFAULT_STACK_CAPACITY;

    $el_poison = el_poison;
    $el_dtor   = el_dtor;
    $el_dump   = el_dump;

    $data      = log_calloc($el_size, $capacity);
    if ($data == nullptr)
    {
        log_error("log_calloc(el_size                = %lu,\n"
                            " DEFAULT_STACK_CAPACITY = %lu) returns nullptr\n", $el_size, DEFAULT_STACK_CAPACITY);
        return false;
    }

    stack_gap_fill_poison(stk);
    stack_debug_verify   (stk);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

stack *stack_new(const size_t el_size, const void *const el_poison                   /* = nullptr */,
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

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void stack_dtor(void *const _stk)
{
    stack *const stk = (stack *) _stk;
    if (stack_verify(stk) != STK_OK) return;

    stack_data_dtor(stk);
    *stk = STK_POISON;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dtor(stack *const stk)
{
    stack_debug_verify(stk);

    if ($el_dtor == nullptr) { log_inline_warning("can't dtor stack elements\n"); }
    else
    {
        for (size_t i = 0; i < $size; ++i) { (*$el_dtor)(stack_get(stk, i)); }
    }

    log_free($data);
}

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_push(stack *const stk, const void *const data)
{
    if (stack_verify(stk) != STK_OK)                             return false;
    if (data == nullptr) { log_error("param data is nullptr\n"); return false; }

    if ($size == $capacity)
    {
        if (stack_resize(stk, 2 * $capacity) == false) return false;
    }

    memcpy(stack_get(stk, $size), data, $el_size);
    $size++;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_pop(stack *const stk, void *const data /* = nullptr */)
{
    if (stack_verify(stk) != STK_OK) return false;
    if ($size == 0)                 { log_error("stack_pop from empty stack\n"); return false; }

    if (data != nullptr) memcpy(data, stack_get(stk, $size - 1), $el_size);
    $size--;
    stack_el_fill_poison(stk, $size);

    if (4 * $size <= $capacity)
    {
        size_t new_capacity = 2 * $size;
        if (new_capacity < DEFAULT_STACK_CAPACITY) new_capacity = DEFAULT_STACK_CAPACITY;
        stack_resize(stk, new_capacity);
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_resize(stack *const stk, const size_t new_capacity)
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

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_front(const stack *const stk, void *const data)
{
    if (stack_verify(stk) != STK_OK)                                    return false;
    if (data == nullptr) { log_error("param data is nullptr\n");        return false; }
    if ($size == 0)      { log_error("stack_front() of empty stack\n"); return false; }

    memcpy(data, stack_get(stk, $size - 1), $el_size);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_is_empty(const stack *const stk)
{
    if (stack_verify(stk) != STK_OK) return false;

    return $size == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_dump(const void *const _stk)
{
    const stack *const stk = (const stack *) _stk;
    if (stack_verify(stk) != STK_OK) return;

    stack_dump(stk, false);
}