#include "array_static.h"

//================================================================================================================================
// ARRAY
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// verify
//--------------------------------------------------------------------------------------------------------------------------------

unsigned _array_verify(const array *const arr)
{
$i
    unsigned err = ARR_OK;

    if (arr == nullptr)
    {
        err = (1 << ARR_NULLPTR);
$       array_log_error(arr, err);

$o      return err;
    }

$   err = err | array_poison_verify(arr);
$   err = err | array_fields_verify(arr);

$   array_log_error(arr, err);
$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_log_error(const array *const arr, const unsigned err)
{
$i
    if (err == ARR_OK) { $o return; }

$   log_error("array verify failed\n");

$   for (size_t i = 1; i * sizeof(char *) < sizeof(ARR_STATUS_MESSAGES); ++i)
    {
        if (err & (1 << i)) log_tab_error_message("%s", "\n", ARR_STATUS_MESSAGES[i]);
    }

$   log_message("\n");

$   array_static_dump(arr, true);

$   log_tab_error_message("====================", "\n");
$   log_message("\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned array_poison_verify(const array *const arr)
{
$i
    log_assert(arr != nullptr);

    unsigned err = ARR_OK;

    if ($data    == ARR_POISON.data   ) err = err | (1 << ARR_POISON_DATA   );
    if ($size    == ARR_POISON.size   ) err = err | (1 << ARR_POISON_SIZE   );
    if ($el_size == ARR_POISON.el_size) err = err | (1 << ARR_POISON_EL_SIZE);
    if ($el_dtor == ARR_POISON.el_dtor) err = err | (1 << ARR_POISON_EL_DTOR);
    if ($el_dump == ARR_POISON.el_dump) err = err | (1 << ARR_POISON_EL_DUMP);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------

static unsigned array_fields_verify(const array *const arr)
{
$i
    log_assert(arr != nullptr);

    unsigned err = ARR_OK;

    if ($data    == nullptr) err = err | (1 << ARR_NULLPTR_DATA);
    if ($size    ==     0UL) err = err | (1 << ARR_ZERO_SIZE   );
    if ($el_size ==     0UL) err = err | (1 << ARR_ZERO_EL_SIZE);

$o  return err;
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

bool array_ctor(array *const arr, const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) /* = nullptr */,
                                                                           void (*el_dump) (const void *const el) /* = nullptr */)
{
$i
    log_verify(arr  != nullptr, false);
    log_verify(size    !=  0UL, false);
    log_verify(el_size !=  0UL, false);

    $size    = size;
    $el_size = el_size;
    $el_dtor = el_dtor;
    $el_dump = el_dump;

$   $data = log_calloc(size, el_size);
    if ($data == nullptr)
    {
$       log_error("log_calloc(size = %lu, el_size = %lu) returns nullptr\n", size, el_size);
$o      return false;
    }

$   arr_debug_verify(arr);
$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

array *array_new(const size_t size, const size_t el_size, void (*el_dtor) (      void *const el) /* = nullptr */,
                                                          void (*el_dump) (const void *const el) /* = nullptr */)
{
$i
$   array *const arr_new = (array *) log_calloc(1, sizeof(array));
    if (arr_new == nullptr)
    {
$       log_error("log_calloc(1, sizoef(array) = %lu) returns nullptr\n", sizeof(array));
$o      return nullptr;
    }

$   if (!array_ctor(arr_new, size, el_size, el_dtor, el_dump)) { log_free(arr_new); $o return nullptr; }

$   arr_debug_verify(arr_new);
$o  return arr_new;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void array_dtor(void *const _arr)
{
$i
    if (_arr == nullptr) { $o return; }

    array *const arr = (array *) _arr;
$   arr_verify(arr, (void) 0);

$   array_data_dtor(arr);
   *arr = ARR_POISON;
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

void array_free(void *const _arr)
{
$i
$   array_dtor(_arr);
$   log_free  (_arr);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_dtor(array *const arr)
{
$i
$   arr_debug_verify(arr);

    if ($el_dtor != nullptr)
    {
        char *const begin = (char *) $data;
        char *const end   = (char *) $data + $size * $el_size;

$       for (char *cur = begin; cur != end; cur += $el_size) $el_dtor(cur);
    }

$   log_free($data);
$o
}

//--------------------------------------------------------------------------------------------------------------------------------
// init
//--------------------------------------------------------------------------------------------------------------------------------

bool array_init(array *const arr, const void *const init_elem)
{
$i
$   arr_verify(arr, false);
    log_verify(init_elem != nullptr, false);

    char *const begin = (char *) $data;
    char *const end   = (char *) $data + $size * $el_size;

$   for (char *cur_elem = begin; cur_elem != end; cur_elem += $el_size)
        memcpy(cur_elem, init_elem, $el_size);

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void array_dump(const void *const _arr)
{
$i
    const array *const arr = (const array *) _arr;
$   arr_verify(arr, (void) 0);

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

    bool are_invalid_fields = are_invalid_public_fields | are_invalid_static_fields;

$   array_data_dump(arr, are_invalid_fields);

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_header_dump(const array *const arr)
{
$i
$   log_tab_service_message("array (address: %p)\n"
                            "{", "\n",      arr);

$   if (arr == nullptr) { log_tab_service_message("}", "\n"); $o return false; }
    LOG_TAB++;

$o  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_public_fields_dump(const array *const arr)
{
$i
    log_assert(arr != nullptr);

    bool is_any_invalid = false;

    if      ($data == ARR_POISON.data)  { $ poison_field_dump("data");                is_any_invalid = true; }
    else if ($data == nullptr)          { $ error_field_dump ("data", "%p", nullptr); is_any_invalid = true; }
    else                                { $ usual_field_dump ("data", "%p",   $data); }

    if      ($size == ARR_POISON.size)  { $ poison_field_dump("size");                is_any_invalid = true; }
    else if ($size == 0UL)              { $ error_field_dump ("size", "%lu",   0UL);  is_any_invalid = true; }
    else                                { $ usual_field_dump ("size", "%lu", $size);  }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool array_static_fields_dump(const array *const arr)
{
$i
    log_assert(arr != nullptr);

    bool is_any_invalid = false;

    if      ($el_size == ARR_POISON.el_size) { $ poison_field_dump ("el_size");                  is_any_invalid = true; }
    else if ($el_size == 0UL)                { $ error_field_dump  ("el_size", "%lu", $el_size); is_any_invalid = true; }
    else                                     { $ usual_field_dump  ("el_size", "%lu", $el_size); }

    if      ($el_dtor == ARR_POISON.el_dtor) { $ poison_field_dump ("el_dtor"); is_any_invalid = true; }
    else if ($el_dtor == nullptr)            { $ warning_field_dump("el_dtor", "%p" , $el_dtor); }
    else                                     { $ usual_field_dump  ("el_dtor", "%p" , $el_dtor); }

    if      ($el_dump == ARR_POISON.el_dump) { $ poison_field_dump ("el_dump"); is_any_invalid = true; }
    else if ($el_dump == nullptr)            { $ warning_field_dump("el_dump", "%p" , $el_dump); }
    else                                     { $ usual_field_dump  ("el_dump", "%p" , $el_dump); }

$   log_message("\n");

$o  return is_any_invalid;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_dump(const array *const arr, const bool are_invalid_fields)
{
$i
    log_assert(arr   != nullptr);
    log_assert($data != nullptr);

$   log_tab_service_message("data\n"
                            "{", "\n");
    LOG_TAB++;

    if      (are_invalid_fields)  { $ log_tab_error_message  ("Can't dump array content because some of fields are invalid", "\n"); }
    else if ($el_dump == nullptr) { $ log_tab_warning_message("Can't dump array content because .el_dump = nullptr"        , "\n"); }
    else
    {
$       array_data_content_dump(arr);
    }

    LOG_TAB--;
$   log_tab_service_message("}", "\n");
$o
}

//--------------------------------------------------------------------------------------------------------------------------------

static void array_data_content_dump(const array *const arr)
{
$i
    log_assert(arr   != nullptr);
    log_assert($data != nullptr);
    log_assert($size    !=  0UL);
    log_assert($el_size !=  0UL);

    const char *arr_elem = (const char *) $data;

$   for (size_t index = 0; index < $size; ++index)
    {
        log_tab_service_message("#%lu", "\n", index);
        $el_dump(arr_elem);

        arr_elem += $el_size;
    }
$o
}
