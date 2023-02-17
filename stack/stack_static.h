/** @file */
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

#include "stack.h"

#include "stack_settings.h"

//================================================================================================================================
// DSL
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// STACK
//--------------------------------------------------------------------------------------------------------------------------------

#define $data           stk->data

#define $el_size        stk->el_size
#define $size           stk->size
#define $capacity       stk->capacity

#define $el_poison      stk->el_poison

#define $el_dtor        stk->el_dtor
#define $el_dump        stk->el_dump

//================================================================================================================================
// GLOBAL
//================================================================================================================================

/**
*   @brief Типы ошибок в стеке
*/
enum STK_STATUS
{
    STK_OK                          , ///< ОК
    STK_NULLPTR                     , ///< stk = nullptr
    STK_NULLPTR_DATA                , ///< .data = nullptr

    STK_POISON_DATA                 , ///< .data         = STK_POISON.data
    STK_POISON_EL_SIZE              , ///< .el_size      = STK_POISON.el_size
    STK_POISON_SIZE                 , ///< .size         = STK_POISON.size
    STK_POISON_CAPACITY             , ///< .capacity     = STK_POISON.capacity
    STK_POISON_EL_POISON            , ///< .el_poison    = STK_POISON.el_poison
    STK_POISON_EL_DTOR              , ///< .el_dtor      = STK_POISON.el_dtor
    STK_POISON_EL_DUMP              , ///< .el_dump      = STK_POISON.el_dump

    STK_NOT_POISON_GAP              , ///< не POISON-значение в неиспользуемой ячейке
    STK_INVALID_SIZE_CAPACITY       , ///< .size > .capacity
};

/**
*   @brief Сообщения для каждого типа ошибок
*
*   @see enum STK_STATUS
*/
static const char *STK_STATUS_MESSAGES[] =
{
    "stack is ok"                           ,
    "stack is nullptr"                      ,
    "stack.data is nullptr"                 ,

    "stack.data is invalid"                 ,
    "stack.size is invalid"                 ,
    "stack.capacity is invalid"             ,
    "stack.el_poison is invalid"            ,
    "stack.el_size is invalid"              ,
    "stack.el_dtor is invalid"              ,
    "stack.el_dump is invalid"              ,

    "stack gap is not poisoned"             ,
    "stack.size more than stack.capacity"   ,
};

/**
*   @brief POISON-значения полей стека
*/
static const stack STK_POISON =
{
    (void *) 0xBADF00D                      , //data

    0xABADBABE                              , //el_size
    0xBADCAB1E                              , //size
    0xDEADBEFF                              , //capacity

    (const void *) 0xCAFED00D               , //el_poison

    (void (*)(      void *const)) 0xFEEDFACE, //el_dtor
    (void (*)(const void *const)) 0xDEADDEAD, //el_dump
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_gap_fill_poison(stack *const stk);

/**
*   @brief Устанавливает POISON-значения в неиспользуемые ячейки стека.
*   Оболочка для back_trace.
*/
static void _stack_gap_fill_poison(const char *const cur_file,
                                   const char *const cur_func,
                                   const int         cur_line,

                                   stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_el_fill_poison(stack *const stk, const size_t filled_index);

/**
*   @brief Заполняет POISON-значения в элемент стека.
*   Оболочка для back_trace.
*/
static void _stack_el_fill_poison(const char *const cur_file,
                                  const char *const cur_func,
                                  const int         cur_line,

                                  stack *const stk, const size_t filled_index);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_gap_is_poison(const stack *const stk);

/**
*   @brief Проверяет наличие POISON-значений в неиспользуемых ячейках стека.
*   Оболочка для back_trace.
*/
static bool _stack_gap_is_poison(const char *const cur_file,
                                 const char *const cur_func,
                                 const int         cur_line,

                                 const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_el_is_poison(const stack *const stk, const size_t check_index);

/**
*   @brief Проверяет наличие POISON-значений в элементе стека.
*   Оболочка для back_trace.
*/
static bool _stack_el_is_poison(const char *const cur_file,
                                const char *const cur_func,
                                const int         cur_line,

                                const stack *const stk, const size_t check_index);

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

static void *_stack_get(const stack *const stk, const size_t index);

/**
*   @brief Возвращает указатель на элемент стека.
*   Оболочка для back_trace.
*/
static void *_stack_get(const char *const cur_file,
                        const char *const cur_func,
                        const int         cur_line,

                        const stack *const stk, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

static unsigned _stack_verify(const stack *const stk);

/**
*   @brief Верификатор стека.
*   Оболочка для back_trace.
*
*   @return битовая маска кодов ошибок из enum STK_STATUS
*
*   @see enum STK_STATUS
*/
static unsigned _stack_verify(const char *const cur_file,
                              const char *const cur_func,
                              const int         cur_line,

                              const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_log_error(const stack *const stk, const unsigned err);

/**
*   @brief Выводит сообщения об ошибках в стеке в лог.
*   Оболочка для back_trace.
*/
static void _stack_log_error(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const stack *const stk, const unsigned err);

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     = nullptr,
                                                                      void (     *el_dtor  )(      void *const) = nullptr,
                                                                      void (     *el_dump  )(const void *const) = nullptr);

static stack *_stack_new(const size_t el_size, const void *const el_poison                     = nullptr,
                                                     void (     *el_dtor  )(      void *const) = nullptr,
                                                     void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_dtor(void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_data_dtor(stack *const stk);

/**
*   @brief Dtor элементов стека.
*   Оболочка для back_trace.
*/
static void _stack_data_dtor(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для функции удаления элемента стека для back_trace.
*/
static void _stack_private_el_dtor(const char *const cur_file,
                                   const char *const cur_func,
                                   const int         cur_line,

                                   void (*el_dtor) (void *const),
                                                    void *const el);

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_push(stack *const stk, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_pop(stack *const stk, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_resize(stack *const stk, const size_t new_capacity);

/**
*   @brief Stack_resize.
*   Оболочка для back_trace.
*/
static bool _stack_resize(const char *const cur_file,
                          const char *const cur_func,
                          const int         cur_line,

                          stack *const stk, const size_t new_capacity);

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_front(const stack *const stk, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

static bool _stack_is_empty(const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_dump(const void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_static_dump(const stack *const stk, const bool is_full);

/**
*   @brief Дамп стека.
*   Оболочка для back_trace.
*
*   @param stk     [in] - стек
*   @param is_full [in] - true, если полный (служебный) дамп, false иначе
*/
static void _stack_static_dump(const char *const cur_file,
                               const char *const cur_func,
                               const int         cur_line,

                               const stack *const stk, const bool is_full);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_public_fields_dump(const stack *const stk);

/**
*   @brief Дамп полей стека.
*   Оболочка для back_trace.
*/
static void _stack_public_fields_dump(const char *const cur_file,
                                      const char *const cur_func,
                                      const int         cur_line,

                                      const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_data_dump(const stack *const stk, const bool is_full);

/**
*   @brief Дамп содержимого стека.
*   Оболочка для back_trace.
*/
static void _stack_data_dump(const char *const cur_file,
                             const char *const cur_func,
                             const int         cur_line,

                             const stack *const stk, const bool is_static);

//--------------------------------------------------------------------------------------------------------------------------------

static void _stack_el_dump(const stack *const stk, const void *const el);

/**
*   @brief Дамп элемента стека.
*   Оболочка для back_trace.
*/
static void _stack_el_dump(const char *const cur_file,
                           const char *const cur_func,
                           const int         cur_line,

                           const stack *const stk, const void *const el);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Оболочка для функции дампа элемента стека для back_trace.
*/
static void _stack_private_el_dump(const char *const cur_file,
                                   const char *const cur_func,
                                   const int         cur_line,

                                   void (*el_dump) (const void *const),
                                                    const void *const el);

//================================================================================================================================
// MACRO DEFINITIONS
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// stack poison
//--------------------------------------------------------------------------------------------------------------------------------

#define stack_gap_fill_poison(stk              ) _stack_gap_fill_poison(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk              )
#define stack_el_fill_poison( stk, filled_index) _stack_el_fill_poison (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, filled_index)
#define stack_gap_is_poison(  stk              ) _stack_gap_is_poison  (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk              )
#define stack_el_is_poison(   stk,  check_index) _stack_el_is_poison   (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk,  check_index)

//--------------------------------------------------------------------------------------------------------------------------------
// stack get
//--------------------------------------------------------------------------------------------------------------------------------

#define stack_get(stk, index) _stack_get(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, index)

//--------------------------------------------------------------------------------------------------------------------------------
// stack verify
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef STACK_NVERIFY
#define stack_verify(stk, ret_val)                                                          \
    if (_stack_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk) != STK_OK)              \
    {                                                                                       \
        return ret_val;                                                                     \
    }
#else
#define stack_verify(stk, ret_val) ;
#endif

#if defined(STACK_DEBUG) && !defined(STACK_NVERIFY)
#define stack_debug_verify(stk)                                                             \
        log_assert(_stack_verify(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk) == STK_OK);
#else
#define stack_debug_verify(stk) ;
#endif

#define stack_log_error(stk, err) _stack_log_error(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, err)

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

#define stack_data_dtor(      stk        ) _stack_data_dtor      (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk        )
#define stack_private_el_dtor(el_dtor, el) _stack_private_el_dtor(__FILE__, __PRETTY_FUNCTION__, __LINE__, el_dtor, el)

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

#define stack_resize(stk, new_capacity) _stack_resize(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, new_capacity)

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

#define stack_static_dump(       stk, is_full) _stack_static_dump       (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, is_full)
#define stack_public_fields_dump(stk         ) _stack_public_fields_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, stk         )
#define stack_data_dump(         stk, is_full) _stack_data_dump         (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk, is_full)
#define stack_el_dump(           stk,      el) _stack_el_dump           (__FILE__, __PRETTY_FUNCTION__, __LINE__, stk,      el)
#define stack_private_el_dump(   el_dump,  el) _stack_private_el_dump   (__FILE__, __PRETTY_FUNCTION__, __LINE__, el_dump,  el)

#endif //STACK_STATIC_H