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
// stack canary
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef STACK_CANARY_PROTECTION

static stk_canary_t stk_get_left_canary(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    return *((const stk_canary_t *) $data - 1);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stk_set_left_canary(stack *const stk, const stk_canary_t value)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    stk_canary_t *left_canary = (stk_canary_t *) $data - 1;
    *left_canary = value;
}

#define stk_set_valid_left_canary(stk)  stk_set_left_canary(stk, L_CANARY);

//--------------------------------------------------------------------------------------------------------------------------------

static stk_canary_t stk_get_right_canary(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    return *(const stk_canary_t *) ((const char *) $data + ($capacity * $el_size));
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stk_set_right_canary(stack *const stk, const stk_canary_t value)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    stk_canary_t *right_canary = (stk_canary_t *) ((char *) $data + ($capacity * $el_size));
    *right_canary = value;
}

#define stk_set_valid_right_canary(stk)  stk_set_right_canary(stk, R_CANARY);

#endif //STACK_CANARY_PROTECTION

//--------------------------------------------------------------------------------------------------------------------------------
// stack hash
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef STACK_HASH_PROTECTION

static stk_hash_t stk_get_actual_hash(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    stk_hash_t  hash_cnt = HASH_BEG;
    size_t stk_size_left = $size;

    const unsigned long long *data_ll = (const unsigned long long *) $data;
    for (; sizeof(*data_ll) <= stk_size_left; stk_size_left -= sizeof(*data_ll))
    {
        hash_cnt = ((hash_cnt << 5) + hash_cnt) + *data_ll;
        data_ll += 1;
    }

    const unsigned char *data_c = (const unsigned char *) data_ll;
    for (; sizeof(*data_c) <= stk_size_left; stk_size_left -= sizeof(*data_c))
    {
        hash_cnt = ((hash_cnt << 5) + hash_cnt) + *data_c;
        data_c  += 1;
    }

    return hash_cnt;
}

//--------------------------------------------------------------------------------------------------------------------------------

static stk_hash_t stk_get_saved_hash(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    #ifdef STACK_CANARY_PROTECTION
    return *(stk_hash_t *) ((char *) $data + ($capacity * $el_size) + sizeof(stk_canary_t));
    #else
    return *(stk_hash_t *) ((char *) $data + ($capacity * $el_size));
    #endif
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stk_set_hash(stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    #ifdef STACK_CANARY_PROTECTION
    stk_hash_t *stk_hash_val = (stk_hash_t *) ((char *) $data + ($capacity * $el_size) + sizeof(stk_canary_t));
    #else
    stk_hash_t *stk_hash_val = (stk_hash_t *) ((char *) $data + ($capacity * $el_size));
    #endif
    *stk_hash_val = stk_get_actual_hash(stk);
}

#endif //STACK_HASH_PROTECTION

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

static void stack_gap_fill_poison(stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    if ($el_poison == nullptr) return;

    for (size_t i = $size; i < $capacity; ++i) stack_el_fill_poison(stk, i);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_fill_poison(stack *const stk, const size_t filled_index)
{
    assert(stk          !=  nullptr);
    assert($data        !=  nullptr);
    assert(filled_index < $capacity);

    if ($el_poison == nullptr) return;

    void *filled_el = (char *) $data + $el_size * filled_index;
    memcpy(filled_el, $el_poison, $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_gap_is_poison(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    if ($el_poison == nullptr) return true;

    for (size_t i = $size; i < $capacity; ++i) if (!stack_el_is_poison(stk, i)) return false;
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_el_is_poison(const stack *const stk, const size_t checking_index)
{
    assert(stk            !=  nullptr);
    assert($data          !=  nullptr);
    assert(checking_index < $capacity);

    if ($el_poison == nullptr) return true;

    const void *checkig_el = (char *) $data + $el_size * checking_index;
    return is_byte_equal(checkig_el, $el_poison, $el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned stack_verify(const stack *const stk,    const char *file,
                                                        const char *func,
                                                        const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    unsigned err = 0;

    if (stk == nullptr) return STK_NULLPTR;

    if ($data         == nullptr                ) err = err | (1 << STK_NULLPTR_DATA       );
    if ($data         == STK_POISON.data        ) err = err | (1 << STK_POISON_DATA        );
    if ($el_size      == STK_POISON.el_size     ) err = err | (1 << STK_POISON_EL_SIZE     );
    if ($size         == STK_POISON.size        ) err = err | (1 << STK_POISON_SIZE        );
    if ($capacity     == STK_POISON.capacity    ) err = err | (1 << STK_POISON_CAPACITY    );
    if ($el_poison    == STK_POISON.el_poison   ) err = err | (1 << STK_POISON_EL_POISON   );
    if ($el_dtor      == STK_POISON.el_dtor     ) err = err | (1 << STK_POISON_EL_DTOR     );
    if ($el_dump      == STK_POISON.el_dump     ) err = err | (1 << STK_POISON_EL_DUMP     );

    if (!stack_gap_is_poison(stk))                err = err | (1 << STK_NOT_POISON_GAP       );
    if ($size > $capacity)                        err = err | (1 << STK_INVALID_SIZE_CAPACITY);

    #ifdef STACK_CANARY_PROTECTION
    if (stk_get_left_canary(stk)  != L_CANARY)    err = err | (1 << STK_L_CANARY_FAILED);
    if (stk_get_right_canary(stk) != R_CANARY)    err = err | (1 << STK_R_CANARY_FAILED);
    #endif
    #ifdef STACK_HASH_PROTECTION
    if (stk_get_actual_hash(stk) != stk_get_saved_hash(stk)) err = err | (1 << STK_HASH_FAILED);
    #endif

    stack_log_error(stk, err, file, func, line);
    return err;
}

#define stack_verify(stk) stack_verify(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__);

#ifdef STACK_DEBUG
#define stack_debug_verify(stk) stack_verify(stk)
#else
#define stack_debug_verify(stk) ;
#endif

//--------------------------------------------------------------------------------------------------------------------------------
// stack static dump
//--------------------------------------------------------------------------------------------------------------------------------

static void stack_log_error(const stack *const stk, const unsigned stk_error,   const char *file,
                                                                                const char *func,
                                                                                const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    if (stk_error == 0) return;

    log_param_error(file, func, line, "stack verify failed\n");
    log_message(HTML_COLOR_DARK_RED);

    for (size_t i = 0; i * sizeof(char *) < sizeof(STK_STATUS_MESSAGES); i++)
    {
        if (stk_error & (1 << i)) log_message("%s\n", STK_STATUS_MESSAGES[i]);
    }
    log_message(HTML_COLOR_CANCEL);

    stack_static_dump(stk, file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_static_dump(const stack *const stk, const char *file,
                                                      const char *func,
                                                      const int   line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_message("stack_static_dump called by error in\n"
                "    FILE: %s\n"
                "FUNCTION: %s\n"
                "    LINE: %d\n\n", file, func, line);

    stack_public_fields_dump(stk);

    if      ($data == nullptr)         { log_message("    data = " HTML_COLOR_DARK_RED "nullptr\n" HTML_COLOR_CANCEL); }
    else if ($data == STK_POISON.data) { log_message("    data = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else
    {
        #ifdef STACK_CANARY_PROTECTION
        stk_canary_t left_canary  = stk_get_left_canary(stk);
        stk_canary_t right_canary = stk_get_right_canary(stk);

        log_message("\n"
                    HTML_COLOR_MEDIUM_BLUE
                    "    l_canary = %llu\n"
                    "    r_canary = %llu\n"
                    HTML_COLOR_CANCEL  "\n", left_canary, right_canary);
        #endif //STACK_CANARY_PROTECTION

        #ifdef STACK_HASH_PROTECTION
        stk_hash_t svaed_hash  = stk_get_saved_hash(stk);
        stk_hash_t actual_hash = stk_get_actual_hash(stk);

        log_message("\n"
                    HTML_COLOR_MEDIUM_BLUE
                    "    saved_hash  = %llu\n"
                    "    actual_hash = %llu\n"
                    HTML_COLOR_CANCEL     "\n", svaed_hash, actual_hash);
        #endif //STACK_HASH_PROTECTION

        log_message("    data (address: %p)\n"
                    "    {\n");

        stack_data_dump(stk);

        for (size_t i = $size; i < $capacity; ++i)
        {
            log_message(HTML_COLOR_MEDIUM_BLUE "        %lu:\n" HTML_COLOR_CANCEL, i);

            if ($el_poison == nullptr || $el_poison == STK_POISON.el_poison)
            {
                log_message(HTML_COLOR_DARK_ORANGE "can't compare stack element with poison-element\n" HTML_COLOR_CANCEL);
            }
            else if (is_byte_equal((char *) $data + i * $el_size, $el_poison, $el_size))
            {
                log_message(HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL);
                continue;
            }

            if ($el_dump == nullptr || $el_poison == STK_POISON.el_dump)
            {
                log_message(HTML_COLOR_DARK_ORANGE "can't dump stack element\n" HTML_COLOR_CANCEL);
            }
            else (*$el_dump) ((char *) $data + i * $el_size);
            log_message("\n");
        }
        log_message("    }\n");
    }
    log_message("}\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_public_fields_dump(const stack *const stk)
{
    log_message("stack (address: %p)\n"
                "{\n",          stk);

    if (stk == nullptr) { log_message("}\n\n"); return; }

    if ($el_size == STK_POISON.el_size)          { log_message("    el_size   = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    el_size   = %lu\n", $el_size); }

    if ($size == STK_POISON.size)                { log_message("    size      = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    size      = %lu\n", $size); }

    if ($capacity == STK_POISON.capacity)        { log_message("    capacity  = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    capacity  = %lu\n", $capacity); }

    if ($el_poison == nullptr)                   { log_message("    el_poison = " HTML_COLOR_DARK_ORANGE "nullptr\n" HTML_COLOR_CANCEL); }
    else if ($el_poison == STK_POISON.el_poison) { log_message("    el_poison = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    el_poison = %p\n", $el_poison); }

    if ($el_dtor == nullptr)                     { log_message("    el_dtor   = " HTML_COLOR_DARK_ORANGE "nullptr\n" HTML_COLOR_CANCEL); }
    else if ($el_poison == STK_POISON.el_dtor)   { log_message("    el_dtor   = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    el_dtor   = %p\n", $el_dtor); }

    if ($el_dump == nullptr)                     { log_message("    el_dump   = " HTML_COLOR_DARK_ORANGE "nullptr\n" HTML_COLOR_CANCEL); }
    else if ($el_poison == STK_POISON.el_poison) { log_message("    el_dump   = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else                                         { log_message("    el_dump   = %p\n", $el_dump); }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dump(const stack *const stk)
{
    assert(stk   != nullptr);
    assert($data != nullptr);

    for (size_t i = 0; i < $size; ++i)
    {
        log_message("        %lu:\n", i);

        if ($el_dump == nullptr || $el_dump == STK_POISON.el_dump)
        {
            log_message(HTML_COLOR_DARK_ORANGE "can't dump stack element\n" HTML_COLOR_CANCEL);
        }
        else (*$el_dump) ((char *) $data + i * $el_size);
        log_message("\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack resize
//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_resize(stack *const stk, const size_t new_capacity)
{
    stack_debug_verify(stk);

    if (new_capacity == $capacity) return true;

    void *old_data = $data;
    #ifdef STACK_CANARY_PROTECTION
    $data = (stk_canary_t *) $data - 1;
    #endif

    #ifdef STACK_CANARY_PROTECTION
    size_t canary_size = 2 * sizeof(stk_canary_t);
    #else
    size_t canary_size = 0;
    #endif

    #ifdef STACK_HASH_PROTECTION
    size_t hash_size = sizeof(stk_hash_t);
    #else
    size_t hash_size = 0;
    #endif

    $data = log_realloc($data, (new_capacity * $el_size) + canary_size + hash_size);
    if ($data == nullptr)
    {
        log_error("log_realloc(.data) returns nullptr");
        $data = old_data;
        return false;
    }

    #ifdef STACK_CANARY_PROTECTION
    $data = (stk_canary_t *) $data + 1;
    #endif
    $capacity = new_capacity;

    #ifdef STACK_CANARY_PROTECTION
    stk_set_valid_left_canary(stk);
    stk_set_valid_right_canary(stk);
    #endif
    #ifdef STACK_HASH_PROTECTION
    stk_set_hash(stk);
    #endif

    stack_gap_fill_poison(stk);

    stack_debug_verify(stk);
    return true;
}

//================================================================================================================================
// USER FUNCTION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack ctor dtor
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison         /* = nullptr */,
                                                        void (*el_dtor) (      void *const) /* = nullptr */,
                                                        void (*el_dump) (const void *const) /* = nullptr */)
{
    if (stk == nullptr) { stack_log_error(stk, STK_NULLPTR, __FILE__, __PRETTY_FUNCTION__, __LINE__); return false; }

    $el_size  = el_size;
    $size     = 0;
    $capacity = DEFAULT_STACK_CAPACITY;

    $el_poison = el_poison;
    
    $el_dtor      = el_dtor;
    $el_dump      = el_dump;

    #ifdef STACK_CANARY_PROTECTION
    size_t canary_size = 2 * sizeof(stk_canary_t);
    #else
    size_t canary_size = 0;
    #endif
    #ifdef STACK_HASH_PROTECTION
    size_t hash_size = sizeof(stk_hash_t);
    #else
    size_t hash_size = 0;
    #endif
    
    $data = log_calloc(1, ($capacity * $el_size) + canary_size + hash_size);
    if ($data == nullptr)
    {
        log_error("log_calloc(<.data>) returns nullptr\n");
        return false;
    }
    #ifdef STACK_CANARY_PROTECTION
    $data = (stk_canary_t *) $data + 1;
    stk_set_valid_left_canary(stk);
    stk_set_valid_right_canary(stk);
    #endif
    #ifdef STACK_HASH_PROTECTION
    stk_set_hash(stk);
    #endif

    stack_gap_fill_poison(stk);

    stack_debug_verify(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *stack_new(const size_t el_size,   const void *const el_poison         /* = nullptr */,
                                        void (*el_dtor) (      void *const) /* = nullptr */,
                                        void (*el_dump) (const void *const) /* = nullptr */)
{
    stack *const stk = (stack *) log_calloc(1, sizeof(stack));
    if (stk == nullptr)
    {
        log_error("log_calloc(1, sizeof(stack)) returns nullptr\n");
        return nullptr;
    }

    if (!stack_ctor(stk, el_size, el_poison, el_dtor, el_dump)) { log_free(stk); return nullptr; }
    return stk;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_dtor(void *const _stk)
{
    stack *const stk = (stack *) _stk;
    stack_verify(stk);

    if ($el_dtor == nullptr || $el_dtor == STK_POISON.el_dtor)
    {
        log_warning("can't dtor stack elements\n");
    }
    else
    {
        for (size_t i = 0; i < $size; ++i) (*$el_dtor) ((char *) $data + i * $el_size);
    }

    #ifdef STACK_CANARY_PROTECTION
    $data = (stk_canary_t *) $data - 1;
    #endif    
    log_free($data);

    $data = STK_POISON.data;
    $el_size  = STK_POISON.el_size;
    $size     = STK_POISON.size;
    $capacity = STK_POISON.capacity;

    $el_poison = STK_POISON.el_poison;

    $el_dtor      = STK_POISON.el_dtor;
    $el_dump      = STK_POISON.el_dump;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack main
//--------------------------------------------------------------------------------------------------------------------------------

bool stack_push(stack *const stk, const void *const new_el)
{
    stack_verify(stk);
    assert      (new_el != nullptr);

    if ($size == $capacity)
    {
        if (!stack_resize(stk, 2 * $capacity)) { stack_debug_verify(stk); return false; }
    }

    void *new_el_place = (char *) $data + ($size * $el_size);
    memcpy(new_el_place, new_el, $el_size);
    $size += 1;

    #ifdef STACK_HASH_PROTECTION
    stk_set_hash(stk);
    #endif

    stack_debug_verify(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_pop(stack *const stk)
{
    stack_verify(stk);

    if ($size == 0) { log_error("stack_pop from empty stack\n"); stack_debug_verify(stk); return false; }

    $size -= 1;
    void *last_el_place = (char *) $data + ($size * $el_size);
    
    if ($el_dtor == nullptr || $el_dtor == STK_POISON.el_dtor) log_warning("can't dtor stack elements\n");
    else (*$el_dtor) (last_el_place);
    stack_el_fill_poison(stk, $size);

    #ifdef STACK_HASH_PROTECTION
    stk_set_hash(stk);
    #endif

    if (4 * $size <= $capacity)
    {
        size_t new_capacity = $capacity / 2;
        if (new_capacity < DEFAULT_STACK_CAPACITY) new_capacity = DEFAULT_STACK_CAPACITY;

        if (!stack_resize(stk, new_capacity)) { stack_debug_verify(stk); return false; }
    }

    stack_debug_verify(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *stack_front(stack *const stk)
{
    stack_verify(stk);

    if ($size == 0) { log_error("stack_front from empty stack\n"); return nullptr; }

    return (char *) $data + ($size - 1) * $el_size;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_empty(stack *const stk)
{
    stack_verify(stk);

    return $size == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------
// stack user dump
//--------------------------------------------------------------------------------------------------------------------------------

void stack_dump(const void *const _stk)
{
    const stack *const stk = (const stack *) _stk;

    stack_public_fields_dump(stk);

    if      ($data == nullptr)         { log_message("    data     = " HTML_COLOR_DARK_RED "nullptr\n" HTML_COLOR_CANCEL); }
    else if ($data == STK_POISON.data) { log_message("    data     = " HTML_COLOR_POISON "POISON\n" HTML_COLOR_CANCEL); }
    else
    {
        log_message("    data (address: %p)\n"
                    "    {\n", $data);
        stack_data_dump(stk);

        log_message("    }\n");
    }
    log_message("}\n\n");
}
