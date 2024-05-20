#include "array_static.h"

//--------------------------------------------------------------------------------------------------------------------------------

unsigned array_verify(const array *const arr)
{
$i
    unsigned err = ARR_OK;

    if (arr == nullptr)
    {
        err = (1 << ARR_NULLPTR);
$       array_log_error(arr, err);
$o      return err;
    }

$   err = array_fields_verify(arr);

$   array_log_error(arr, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_log_error(const array *const arr, const unsigned err)
{
$i
    if (err == ARR_OK) { $o return; }

$   LOG_ERROR("array verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(ARR_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i))
            LOG_TAB_ERROR_MESSAGE("%s", "\n", ARR_STATUS_MESSAGES[i]);
    }

$   LOG_MESSAGE("\n");
$   array_static_dump(arr, true);
$   LOG_TAB_ERROR_MESSAGE(BOLD_LOG_SEP, "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned array_fields_verify(const array *const arr)
{
$i
    LOG_ASSERT(arr != nullptr);

    unsigned err = ARR_OK;

    #define poison_verify(field, status) \
    if (arr->field == ARR_POISON.field) err |= (1 << status);

    poison_verify(data   , ARR_POISON_DATA)
    poison_verify(size   , ARR_POISON_SIZE)
    poison_verify(el_size, ARR_POISON_EL_SIZE)
    poison_verify(el_dtor, ARR_POISON_EL_DTOR)
    poison_verify(el_dump, ARR_POISON_EL_DUMP)

    #undef poison_verify

    if (arr->data    == nullptr) err |= (1 << ARR_NULLPTR_DATA);
    if (arr->size    ==     0UL) err |= (1 << ARR_ZERO_SIZE   );
    if (arr->el_size ==     0UL) err |= (1 << ARR_ZERO_EL_SIZE);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool array_ctor(array *const arr, const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) /* = nullptr */,
                                                                           void (*el_dump) (const void *const el) /* = nullptr */)
{
$i
    LOG_VERIFY(arr  != nullptr, false);
    LOG_VERIFY(size    !=  0UL, false);
    LOG_VERIFY(el_size !=  0UL, false);

    arr->size    = size;
    arr->el_size = el_size;
    arr->el_dtor = el_dtor;
    arr->el_dump = el_dump;

$   arr->data = LOG_CALLOC(size, el_size);
    if (arr->data == nullptr)
    {
$       LOG_ERROR("log_calloc(size = %lu, el_size = %lu) returns nullptr\n",
                              size,       el_size);
$o      return false;
    }

$   ARRAY_ASSERT(arr);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

array *array_new(const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) /* = nullptr */,
                                                          void (*el_dump) (const void *const el) /* = nullptr */)
{
$i
$   array *arr = (array *) LOG_CALLOC(1, sizeof(array));
    if (arr == nullptr)
    {
$       LOG_ERROR("log_calloc(1, sizeof(array) = %lu) returns nullptr\n", sizeof(array));
$o      return nullptr;
    }
$   if (!array_ctor(arr, size, el_size, el_dtor, el_dump))
    {
        LOG_FREE(arr);
$o      return nullptr;
    }

$   ARRAY_ASSERT(arr);
$o  return arr;
}

//--------------------------------------------------------------------------------------------------------------------------------

void array_dtor(void *const _arr)
{
$i
    if (_arr == nullptr) { $o return; }

    array *const arr = (array *) _arr;
$   ARRAY_VERIFY(arr, (void) 0);

$   array_data_dtor(arr);
    *arr = ARR_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void array_delete(void *const _arr)
{
$i
$   array_dtor(_arr);
$   LOG_FREE  (_arr);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_dtor(array *const arr)
{
$i
$   ARRAY_ASSERT(arr);

    if (arr->el_dtor != nullptr)
    {
        char *const begin = ARRAY_GET(arr, 0);
        char *const end   = ARRAY_GET(arr, arr->size);

$       for (char *cur = begin; cur != end; cur += arr->el_size)
            arr->el_dtor(cur);
    }

$   LOG_FREE(arr->data);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

bool array_init(array *const arr, const void *const init_elem)
{
$i
$   ARRAY_VERIFY(arr, false);
    LOG_VERIFY  (init_elem != nullptr, false);

    char *const begin = ARRAY_GET(arr, 0);
    char *const end   = ARRAY_GET(arr, arr->size);

$   for (char *cur_elem = begin; cur_elem != end; cur_elem += arr->el_size)
        memcpy(cur_elem, init_elem, arr->el_size);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool array_set(array *const arr, const size_t index, const void *const elem)
{
$i
$   ARRAY_VERIFY(arr              , false);
    LOG_VERIFY  (index < arr->size, false);
    LOG_VERIFY  (elem != nullptr  , false);

$   memcpy(ARRAY_GET(arr, index), elem, arr->el_size);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void *array_get(const array *const arr, const size_t index)
{
$i
$   ARRAY_VERIFY(arr,               nullptr);
    LOG_VERIFY  (index < arr->size, nullptr);

$o  return ARRAY_GET(arr, index);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *array_begin(const array *const arr)
{
$i
$   ARRAY_VERIFY(arr, nullptr);
$o  return ARRAY_GET(arr, 0);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *array_end(const array *const arr)
{
$i
$   ARRAY_VERIFY(arr, nullptr);
$o  return ARRAY_GET(arr, arr->size);
}

//--------------------------------------------------------------------------------------------------------------------------------

void array_dump(const void *const _arr)
{
$i
    const array *arr = (const array *) _arr;
$   ARRAY_VERIFY(arr, (void) 0);
$   array_static_dump(arr, false);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_static_dump(const array *const arr, const bool is_full)
{
$i
$   if (!array_header_dump(arr)) { $o return; }

$   bool are_invalid_public_fields =           array_public_fields_dump(arr);
$   bool are_invalid_static_fields = is_full ? array_static_fields_dump(arr) : false;
    bool are_invalid_fields        = are_invalid_public_fields | are_invalid_static_fields;

$   array_data_dump(arr, are_invalid_fields);

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_header_dump(const array *const arr)
{
$i
$   LOG_TAB_SERVICE_MESSAGE("array (address: %p)\n"
                            "{", "\n", arr);

$   if (arr == nullptr)
    {
        LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o      return false;
    }

    LOG_TAB++;
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_public_fields_dump(const array *const arr)
{
$i
    LOG_ASSERT(arr != nullptr);

    bool is_any_invalid = false;

    if      (arr->data == ARR_POISON.data)  { $ POISON_FIELD_DUMP("data");                is_any_invalid = true; }
    else if (arr->data == nullptr)          { $ ERROR_FIELD_DUMP ("data", "%p", nullptr); is_any_invalid = true; }
    else                                    { $ USUAL_FIELD_DUMP ("data", "%p", arr->data); }

    if      (arr->size == ARR_POISON.size)  { $ POISON_FIELD_DUMP("size");                is_any_invalid = true; }
    else if (arr->size == 0UL)              { $ ERROR_FIELD_DUMP ("size", "%lu",   0UL);  is_any_invalid = true; }
    else                                    { $ USUAL_FIELD_DUMP ("size", "%lu", arr->size);  }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_static_fields_dump(const array *const arr)
{
$i
    LOG_ASSERT(arr != nullptr);

    bool is_any_invalid = false;

    if      (arr->el_size == ARR_POISON.el_size) { $ POISON_FIELD_DUMP ("el_size");                      is_any_invalid = true; }
    else if (arr->el_size == 0UL)                { $ ERROR_FIELD_DUMP  ("el_size", "%lu", arr->el_size); is_any_invalid = true; }
    else                                         { $ USUAL_FIELD_DUMP  ("el_size", "%lu", arr->el_size); }

    if      (arr->el_dtor == ARR_POISON.el_dtor) { $ POISON_FIELD_DUMP ("el_dtor"); is_any_invalid = true; }
    else if (arr->el_dtor == nullptr)            { $ WARNING_FIELD_DUMP("el_dtor", "%p" , nullptr); }
    else                                         { $ USUAL_FIELD_DUMP  ("el_dtor", "%p" , arr->el_dtor); }

    if      (arr->el_dump == ARR_POISON.el_dump) { $ POISON_FIELD_DUMP ("el_dump"); is_any_invalid = true; }
    else if (arr->el_dump == nullptr)            { $ WARNING_FIELD_DUMP("el_dump", "%p" , nullptr); }
    else                                         { $ USUAL_FIELD_DUMP  ("el_dump", "%p" , arr->el_dump); }

$   LOG_MESSAGE("\n");
$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_dump(const array *const arr, const bool is_any_invalid)
{
$i
    LOG_ASSERT(arr       != nullptr);
    LOG_ASSERT(arr->data != nullptr);

$   LOG_TAB_SERVICE_MESSAGE("data\n" "{", "\n");
    LOG_TAB++;

    if      (is_any_invalid)          { $ LOG_TAB_ERROR_MESSAGE  ("can't dump array content because some of fields are invalid", "\n"); }
    else if (arr->el_dump == nullptr) { $ LOG_TAB_WARNING_MESSAGE("can't dump array content because .el_dump = nullptr"        , "\n"); }
    else                              { $ array_data_content_dump(arr); }

    LOG_TAB--;
$   LOG_TAB_SERVICE_MESSAGE("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_content_dump(const array *const arr)
{
$i
    LOG_ASSERT(arr          != nullptr);
    LOG_ASSERT(arr->data    != nullptr);
    LOG_ASSERT(arr->size    !=  0UL);
    LOG_ASSERT(arr->el_size !=  0UL);

    const char *arr_elem = (const char *) arr->data;

$   for (size_t i = 0; i < arr->size; ++i)
    {
        LOG_TAB_SERVICE_MESSAGE("#%lu:\n" "{", "\n", i);
        LOG_TAB++;
        arr->el_dump(arr_elem);
        LOG_TAB--;
        LOG_TAB_SERVICE_MESSAGE("}", "\n");

        arr_elem += arr->el_size;
    }
$o
}
