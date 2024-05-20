#include "stack_static.h"

//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// STACK
//--------------------------------------------------------------------------------------------------------------------------------

static void stack_gap_fill_poison(stack *const stk)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);

    if (stk->el_poison == nullptr) return;
    if (stk->el_poison == STK_POISON.el_poison) return;

   for (size_t i = stk->size; i < stk->capacity; ++i) { stack_el_fill_poison(stk, i); }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_fill_poison(stack *const stk, const size_t filled_index)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);
    LOG_ASSERT(filled_index < stk->capacity);

    LOG_ASSERT(stk->el_poison !=              nullptr);
    LOG_ASSERT(stk->el_poison != STK_POISON.el_poison);

    memcpy(stack_get(stk, filled_index), stk->el_poison, stk->el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_gap_is_poison(const stack *const stk)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);

    if (stk->el_poison == nullptr) return true;
    if (stk->el_poison == STK_POISON.el_poison) return true;

    for (size_t i = stk->size; i < stk->capacity; ++i)
        if (!stack_el_is_poison(stk, i))
            return false;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_el_is_poison(const stack *const stk, const size_t check_index)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);
    LOG_ASSERT(check_index < stk->capacity);

    LOG_ASSERT(stk->el_poison !=              nullptr);
    LOG_ASSERT(stk->el_poison != STK_POISON.el_poison);

    return strncmp((const char *) stack_get(stk, check_index), (const char *) stk->el_poison, stk->el_size) == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void *stack_get(const stack *const stk, const size_t index)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);
    LOG_ASSERT(index < stk->capacity);

    return (char *) stk->data + (index * stk->el_size);
}

//--------------------------------------------------------------------------------------------------------------------------------

unsigned stack_verify(const stack *const stk)
{
    unsigned err = STK_OK;

    if (stk == nullptr)
    {
        err = (1 << STK_NULLPTR);
        stack_log_error(stk, err);
        return err;
    }

    err = stack_fields_verify(stk);
    if (err == STK_OK && !stack_gap_is_poison(stk)) err |= (1 << STK_NOT_POISON_GAP);

    stack_log_error(stk, err);
    return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_log_error(const stack *const stk, const unsigned err)
{
    if (err == STK_OK) return;

    LOG_ERROR("stack verify failed\n");

    for (size_t i = 0; i * sizeof(char *) < sizeof(STK_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i))
            LOG_TAB_ERROR_MESSAGE("%s", "\n", STK_STATUS_MESSAGES[i]);
    }

    LOG_MESSAGE("\n");
    stack_static_dump(stk, true);
    LOG_TAB_ERROR_MESSAGE(BOLD_LOG_SEP, "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned stack_fields_verify(const stack *const stk)
{
    LOG_ASSERT(stk != nullptr);

    unsigned err = STK_OK;

    #define poison_verify(field, status) \
        if (stk->field == STK_POISON.field) err |= (1 << status);

    poison_verify(data     , STK_POISON_DATA)
    poison_verify(el_size  , STK_POISON_EL_SIZE)
    poison_verify(size     , STK_POISON_SIZE)
    poison_verify(capacity , STK_POISON_CAPACITY)
    poison_verify(el_poison, STK_POISON_EL_POISON)
    poison_verify(el_dtor  , STK_POISON_EL_DTOR)
    poison_verify(el_dump  , STK_POISON_EL_DUMP)

    #undef poison_verify

    if (err != STK_OK) return err;

    if (stk->data == nullptr)      err |= (1 << STK_NULLPTR_DATA);
    if (stk->size > stk->capacity) err |= (1 << STK_INVALID_SIZE_CAPACITY);

    return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                              void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                              void (     *el_dump  )(const void *const) /* = nullptr */,

                                                              const size_t stack_capacity /* = DEFAULT_STACK_CAPACITY */)
{
    LOG_VERIFY(stk != nullptr      , false);
    LOG_VERIFY(el_size        > 0UL, false);
    LOG_VERIFY(stack_capacity > 0UL, false);

    stk->el_size   = el_size;
    stk->size      = 0;
    stk->capacity  = stack_capacity;

    stk->el_poison = el_poison;
    stk->el_dtor   = el_dtor;
    stk->el_dump   = el_dump;

    stk->data      = LOG_CALLOC(el_size, stack_capacity);
    if (stk->data == nullptr)
    {
        LOG_ERROR("log_calloc(el_size = %lu, stack_capacity = %lu) returns nullptr\n",
                              el_size      , stack_capacity);
        return false;
    }

    stack_gap_fill_poison(stk);
    STACK_ASSERT(stk);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

stack *stack_new(const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                             void (     *el_dtor  )(      void *const) /* = nullptr */,
                                             void (     *el_dump  )(const void *const) /* = nullptr */,

                                             const size_t stack_capacity /* = DEFAULT_STACK_CAPACITY */)
{
    stack *stk = (stack *) LOG_CALLOC(1, sizeof(stack));
    if (stk == nullptr)
    {
        LOG_ERROR("log_calloc(1, sizeof(stack) = %lu) returns nullptr\n", sizeof(stack));
        return nullptr;
    }
    if (!stack_ctor(stk, el_size, el_poison, el_dtor, el_dump, stack_capacity))
    {
        LOG_FREE(stk);
        return nullptr;
    }

    STACK_ASSERT(stk);
    return stk;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_dtor(void *const _stk)
{
    if (_stk == nullptr) return;

    stack *const stk = (stack *) _stk;
    STACK_VERIFY(stk, (void) 0);

    stack_data_dtor(stk);
    *stk = STK_POISON;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_delete(void *const _stk)
{
    stack_dtor(_stk);
    LOG_FREE  (_stk);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dtor(stack *const stk)
{
    STACK_ASSERT(stk);

    if (stk->el_dtor != nullptr)
    {
        for (size_t i = 0; i < stk->size; ++i) stk->el_dtor(stack_get(stk, i));
    }

    LOG_FREE(stk->data);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_push(stack *const stk, const void *const data)
{
    STACK_VERIFY(stk, false);
    LOG_VERIFY  (data != nullptr, false);

    if (stk->size == stk->capacity)
    {
        if (stack_resize(stk, 2 * stk->capacity) == false)
            return false;
    }

    memcpy(stack_get(stk, stk->size), data, stk->el_size);
    stk->size++;

    STACK_ASSERT(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_pop(stack *const stk, void *const data /* = nullptr */)
{
    STACK_VERIFY(stk, false);
    LOG_VERIFY  (stk->size != 0, false);

    if (data != nullptr) memcpy(data, stack_get(stk, stk->size - 1), stk->el_size);
    stk->size--;

    if (stk->el_dtor   != nullptr) stk->el_dtor(stack_get(stk, stk->size));
    if (stk->el_poison != nullptr) stack_el_fill_poison(stk, stk->size);

    if (4 * stk->size <= stk->capacity)
    {
        size_t new_capacity = 2 * stk->size;
        new_capacity = (new_capacity < DEFAULT_STACK_CAPACITY) ? DEFAULT_STACK_CAPACITY : new_capacity;

        stack_resize(stk, new_capacity);
    }

    STACK_ASSERT(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_resize(stack *const stk, const size_t new_capacity)
{
    STACK_ASSERT(stk);

    if (new_capacity == stk->capacity)
        return true;

    void *old_data = stk->data;
    stk->data = LOG_RECALLOC(stk->data, stk->capacity * stk->el_size, new_capacity * stk->el_size);
    if (stk->data == nullptr)
    {
        LOG_ERROR("log_recalloc(stack.data, (capacity = %lu) * (el_size = %lu), (new_capacity = %lu) * (el_size = %lu)) returns nullptr",
                                        stk->capacity,     stk->el_size,         new_capacity,     stk->el_size);
        stk->data = old_data;
        return false;
    }

    stk->capacity = new_capacity;
    stack_gap_fill_poison(stk);

    STACK_ASSERT(stk);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_front(const stack *const stk, void *const data)
{
    STACK_VERIFY(stk, false);
    LOG_VERIFY  (data != nullptr, false);
    LOG_VERIFY  (stk->size != 0 , false);

    memcpy(data, stack_get(stk, stk->size - 1), stk->el_size);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool stack_is_empty(const stack *const stk)
{
    STACK_VERIFY(stk, false);
    return stk->size == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

void stack_dump(const void *const _stk)
{
    const stack *stk = (const stack *) _stk;
    STACK_VERIFY(stk, (void) 0);
    stack_static_dump(stk, false);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_static_dump(const stack *const stk, const bool is_full)
{
    if (!stack_header_dump(stk)) return;

    bool are_invalid_public_fields =           stack_public_fields_dump(stk);
    bool are_invalid_static_fields = is_full ? stack_static_fields_dump(stk) : false;
    bool are_invalid_fields        = are_invalid_public_fields || are_invalid_static_fields;

    stack_data_dump(stk, is_full, are_invalid_fields);

    LOG_TAB--;
    LOG_TAB_SERVICE_MESSAGE("}", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_header_dump(const stack *const stk)
{
    LOG_TAB_SERVICE_MESSAGE("stack (address: %p)\n"
                            "{", "\n", stk);

    if (stk == nullptr)
    {
        LOG_TAB_SERVICE_MESSAGE("}", "\n");
        return false;
    }

    LOG_TAB++;
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_public_fields_dump(const stack *const stk)
{
    LOG_ASSERT(stk != nullptr);

    bool is_any_invalid = false;

    if      (stk->el_size   == STK_POISON.el_size)   { POISON_FIELD_DUMP ("el_size  "); is_any_invalid = true; }
    else                                             { USUAL_FIELD_DUMP  ("el_size  ", "%lu", stk->el_size); }

    if      (stk->size      == STK_POISON.size)      { POISON_FIELD_DUMP ("size     "); is_any_invalid = true; }
    else                                             { USUAL_FIELD_DUMP  ("size     ", "%lu", stk->size); }

    if      (stk->capacity  == STK_POISON.capacity)  { POISON_FIELD_DUMP ("capacity "); is_any_invalid = true; }
    else if (stk->capacity  <  stk->size)            { ERROR_FIELD_DUMP  ("capacity ", "%lu", stk->capacity); is_any_invalid = true; }
    else                                             { USUAL_FIELD_DUMP  ("capacity ", "%lu", stk->capacity); }

    if      (stk->el_poison == STK_POISON.el_poison) { POISON_FIELD_DUMP ("el_poison"); is_any_invalid = true; }
    else if (stk->el_poison == nullptr)              { WARNING_FIELD_DUMP("el_poison", "%p", stk->el_poison); }
    else                                             { USUAL_FIELD_DUMP  ("el_poison", "%p", stk->el_poison); }

    if      (stk->el_dtor   == STK_POISON.el_dtor)   { POISON_FIELD_DUMP ("el_dtor  "); is_any_invalid = true; }
    else if (stk->el_dtor   == nullptr)              { WARNING_FIELD_DUMP("el_dtor  ", "%p", stk->el_dtor); }
    else                                             { USUAL_FIELD_DUMP  ("el_dtor  ", "%p", stk->el_dtor); }

    if      (stk->el_dump   == STK_POISON.el_dump)   { POISON_FIELD_DUMP ("el_dump  "); is_any_invalid = true; }
    else if (stk->el_dump   == nullptr)              { WARNING_FIELD_DUMP("el_dump  ", "%p", stk->el_dump); }
    else                                             { USUAL_FIELD_DUMP  ("el_dump  ", "%p", stk->el_dump); }

    LOG_MESSAGE("\n");
    return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool stack_static_fields_dump(const stack *const stk)
{
    LOG_ASSERT(stk != nullptr);

    bool is_any_invalid = false;

    if      (stk->data == STK_POISON.data) { POISON_FIELD_DUMP("data"); is_any_invalid = true; }
    else if (stk->data == nullptr)         { ERROR_FIELD_DUMP ("data", "%p", nullptr); is_any_invalid = true; }
    else                                   { USUAL_FIELD_DUMP ("data", "%p", stk->data); }

    LOG_MESSAGE("\n");
    return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_dump(const stack *const stk, const bool is_full, const bool are_invalid_fields)
{
    LOG_ASSERT(stk != nullptr);

    LOG_TAB_SERVICE_MESSAGE("data\n" "{", "\n");
    LOG_TAB++;

    if (are_invalid_fields)
    {
        LOG_TAB_ERROR_MESSAGE("can't dump it because some of fields are invalid", "\n");
    }
    else if (is_full) stack_data_debug_dump (stk);
    else              stack_data_pretty_dump(stk);

    LOG_TAB--;
    LOG_TAB_SERVICE_MESSAGE("}", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_debug_dump(const stack *const stk)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);

    stack_data_pretty_dump(stk);

    for (size_t i = stk->size; i < stk->capacity; ++i)
    {
        LOG_TAB_SERVICE_MESSAGE("#%lu:\n" "{", "\n", i);
        LOG_TAB++;
        stack_el_dump(stk, stack_get(stk, i));
        LOG_TAB--;
        LOG_TAB_SERVICE_MESSAGE("}", "\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_data_pretty_dump(const stack *const stk)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);

    for (size_t i = 0; i < stk->size; ++i)
    {
        LOG_TAB_OK_MESSAGE("#%lu:\n" "{", "\n", i);
        LOG_TAB++;
        stack_el_dump(stk, stack_get(stk, i));
        LOG_TAB--;
        LOG_TAB_OK_MESSAGE("}", "\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static void stack_el_dump(const stack *const stk, const void *const el)
{
    LOG_ASSERT(stk       != nullptr);
    LOG_ASSERT(stk->data != nullptr);
    LOG_ASSERT(el        != nullptr);

    if (stk->el_poison != nullptr)
    {
        if (strncmp((const char *) stk->el_poison, (const char *) el, stk->el_size) == 0)
        {
            LOG_TAB_POISON_MESSAGE("POISON", "\n");
            return;
        }
    }

    if (stk->el_dump != nullptr) stk->el_dump(el);
    else LOG_TAB_WARNING_MESSAGE("can't dump stack element", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// VECTOR
//--------------------------------------------------------------------------------------------------------------------------------

/*
*   Далее определны функции структуры данных "vector", которая по сути является стеком (typedef stack vector),
*   но дополнительно предоставляет возможность обращения/изменения любых элементов, а не только верхнего.
*
*   Большинство функций вектора являются inline оболочками для функций стека, поэтому определны в файле "../vector/vector.h",
*   который включен в "../vector/vector_static.h".
*
*   Остальные функции определены ниже.
*/

#include "../vector/vector_static.h"

//--------------------------------------------------------------------------------------------------------------------------------

void *vector_get(const vector *const vec, const size_t index)
{
    VECTOR_VERIFY(vec, nullptr);
    LOG_VERIFY   (index < vec->size, nullptr);
    return VECTOR_GET(vec, index);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *vector_begin(const vector *const vec)
{
    VECTOR_VERIFY(vec, nullptr);
    return (vec->size == 0) ? nullptr : vec->data;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *vector_end(const vector *const vec)
{
    VECTOR_VERIFY(vec, nullptr);
    return (vec->size == 0) ? nullptr : VECTOR_GET(vec, vec->size);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool vector_set(vector *const vec, const size_t index, const void *const data)
{
    VECTOR_VERIFY(vec              , false);
    LOG_VERIFY   (index < vec->size, false);
    LOG_VERIFY   (data != nullptr  , false);

    void *el = VECTOR_GET(vec, index);
    memcpy(el, data, vec->el_size);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool vector_resize(vector *const vec, const size_t count)
{
    VECTOR_VERIFY(vec, false);

    if (count == vec->size) return true;

    vec->size = count;
    bool result = true;

    if (count > vec->capacity) result = stack_resize(vec, count);
    else
    {
        if (vec->el_dtor != nullptr)
        {
            for (size_t ind = count; ind < vec->size; ++ind)
                vec->el_dtor(VECTOR_GET(vec, ind));
        }

        if  (4 * count < vec->capacity) result = stack_resize(vec, count * 2);
    }

    return result;
}
