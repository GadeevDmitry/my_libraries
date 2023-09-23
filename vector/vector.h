/** @file */
#ifndef VECTOR_H
#define VECTOR_H

#include "../stack/stack.h"
#include "logs/log.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура данных вектор.
*   Реализована в виде стека с возможностями обращения/изменения любых элементов вектора.
*/
typedef stack vector;

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Vector_ctor.
*
*   @param vec             [out] - указатель на вектор
*   @param el_size         [in]  - размер элемента вектора
*   @param el_poison       [in]  - указатель на POISON-элемент вектора
*   @param el_dtor         [in]  - указатель на dtor элемента вектора
*   @param el_dump         [in]  - указатель на dump элемента вектора
*   @param vector_capacity [in]  - начальная емкость вектора
*/
static inline bool vector_ctor(vector *const vec, const size_t el_size, const void *const el_poison                     = nullptr,
                                                                              void (     *el_dtor  )(      void *const) = nullptr,
                                                                              void (     *el_dump  )(const void *const) = nullptr,

                                                                              const size_t vector_capacity = DEFAULT_STACK_CAPACITY);

static inline bool vector_ctor(vector *const vec, const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                                              void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                                              void (     *el_dump  )(const void *const) /* = nullptr */,

                                                                              const size_t vector_capacity /* = DEFAULT_STACK_CAPACITY */)
{
    return stack_ctor(vec, el_size, el_poison, el_dtor, el_dump, vector_capacity);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает вектор в динамической памяти.
*
*   @param el_size         [in] - размер элемента вектора
*   @param el_poison       [in] - указатель на POISON-элемент вектора
*   @param el_dtor         [in] - указатель на dtor элемента вектора
*   @param el_dump         [in] - указатель на dump элемента вектора
*   @param vector_capacity [in] - начальная емкость вектора
*
*   @return указатель на созданный вектор или nullptr в случае ошибки
*/
static inline vector *vector_new(const size_t el_size, const void *const el_poison                     = nullptr,
                                                             void (     *el_dtor  )(      void *const) = nullptr,
                                                             void (     *el_dump  )(const void *const) = nullptr,

                                                             const size_t vector_capacity = DEFAULT_STACK_CAPACITY);

static inline vector *vector_new(const size_t el_size, const void *const el_poison                     /* = nullptr */,
                                                             void (     *el_dtor  )(      void *const) /* = nullptr */,
                                                             void (     *el_dump  )(const void *const) /* = nullptr */,

                                                             const size_t vector_capacity /* = DEFAULT_STACK_CAPACITY */)
{
    return stack_new(el_size, el_poison, el_dtor, el_dump, vector_capacity);
}

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Vector_dtor.
*
*   @param _vec [in] - указатель на вектор
*/
static inline void vector_dtor(void *const _vec);
static inline void vector_dtor(void *const _vec)
{
    return stack_dtor(_vec);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Вызывает vector_dtor(void* _vec) и освобождает динамическую память, на которую должен указывать вектор.
*
*   @see vector_dtor(void* _vec)
*/
static inline void vector_free(void *const _vec);
static inline void vector_free(void *const _vec)
{
    return stack_free(_vec);
}

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Кладет элемент вектора в конец.
*
*   @param vec  [in, out] - указатель на вектор
*   @param data [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
static inline bool vector_push_back(vector *const vec, const void *const data);
static inline bool vector_push_back(vector *const vec, const void *const data)
{
    return stack_push(vec, data);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет последний элемент вектора.
*
*   @param vec  [in, out] - указатель на вектор
*   @param data [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
static inline bool vector_pop_back(vector *const vec, void *const data = nullptr);
static inline bool vector_pop_back(vector *const vec, void *const data /* = nullptr */)
{
    return stack_pop(vec, data);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент вектора.
*
*   @param vec     [in, out] - указатель на вектор
*   @param index        [in] - индекс удаляемого элемента
*   @param erased_data [out] - указатель, по которому скопировать содержимое удаляемой вершины (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool vector_erase(vector *const vec, const size_t index, void *const erased_data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает указатель на элемент вектора.
*
*   @param vec   [in] - указатель на вектор
*   @param index [in] - индекс элемента вектора
*
*   @return true, если все ОК, false в случае ошибки
*/
void *vector_get(const vector *const vec, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает указатель на начало вектора - его первый элемент или nullptr, если он пуст.
*/
void *vector_begin(const vector *const vec);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Возвращает указатель на конец вектора - следующий за его последним элементом или nullptr, если он пуст.
*/
void *vector_end(const vector *const vec);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Инициализирует элемент вектора.
*
*   @param vec   [in] - указатель на вектор
*   @param index [in] - индекс элемента вектора
*   @param data  [in] - указатель на содержимое, которым инциализировать элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool vector_set(vector *const vec, const size_t index, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Меняет размер вектора.
*   Если новый размер равен старому, ничего не происходит.
*   Если новый размер больше старого, инициализирует новую память нулями.
*   Если новый размер меньше старого, отбрасываются последние элементы.
*
*   @param vec   [in] - указатель на вектор
*   @param count [in] - новый размер вектора
*
*   @return false в случае ошибки, true иначе
*
*/
bool vector_resize(vector *const vec, const size_t count);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет, пустой ли вектор.
*
*   @param vec [in] - указатель на вектор
*
*   @return true, если вектор пустой, false иначе, или в случае ошибки
*/
static bool inline vector_is_empty(const vector *const vec);
static bool inline vector_is_empty(const vector *const vec)
{
    return stack_is_empty(vec);
}

//--------------------------------------------------------------------------------------------------------------------------------
// vector dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief vector_dump.
*
*   @param _vec [in] - указатель на вектор
*/
void vector_dump(const void *const _vec);

//================================================================================================================================
// MACRO
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// vector verify
//--------------------------------------------------------------------------------------------------------------------------------

#define vec_verify(      vec, ret_val) stk_verify      (vec, ret_val)
#define vec_debug_verify(vec)          stk_debug_verify(vec)

#endif // VECTOR_H
