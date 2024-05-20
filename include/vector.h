/** @file */
#ifndef VECTOR_H
#define VECTOR_H

#include "stack.h"

//================================================================================================================================

/**
*   @brief Структура данных вектор.
*   Реализована в виде стека с возможностями обращения/изменения любых элементов вектора.
*/
typedef stack vector;

//================================================================================================================================


#if defined(NVERIFY)
#define VECTOR_NVERIFY
#endif

#if defined(NDEBUG)
#define VECTOR_NDEBUG
#endif

//--------------------------------------------------------------------------------------------------------------------------------

#ifndef VECTOR_NVERIFY
#define VECTOR_VERIFY(vec, ret_val) \
    if (stack_verify(vec) != 0)     \
    {                               \
        return ret_val;             \
    }

#else
#define VECTOR_VERIFY(stk, ret_val)
#endif

#ifndef VECTOR_NDEBUG
#define VECTOR_ASSERT(vec)                              \
    if (stack_verify(stk) != 0)                         \
    {                                                   \
        fprintf(stderr, "VECTOR ASSERTION FAILED\n");   \
        abort();                                        \
    }

#else
#define VECTOR_ASSERT(stk)
#endif

//================================================================================================================================

/**
*   @brief Конструктор вектора.
*
*   @param vec             [out] - указатель на вектор
*   @param el_size         [in]  - размер элемента вектора
*   @param el_poison       [in]  - указатель на POISON-элемент вектора
*   @param el_dtor         [in]  - указатель на dtor элемента вектора
*   @param el_dump         [in]  - указатель на dump элемента вектора
*   @param vector_capacity [in]  - начальная емкость вектора
*
*   @return true, если удалось создать вектор, false - иначе.
*/
inline bool vector_ctor(vector *const vec, const size_t el_size, const void *const el_poison                     = nullptr,
                                                                       void (     *el_dtor  )(      void *const) = nullptr,
                                                                       void (     *el_dump  )(const void *const) = nullptr,

                                                                 const size_t vector_capacity = DEFAULT_STACK_CAPACITY)
{
    return stack_ctor(vec, el_size, el_poison, el_dtor, el_dump, vector_capacity);
}

/**
*   @brief Создает вектор в динамической памяти.
*
*   @param el_size         [in] - размер элемента вектора
*   @param el_poison       [in] - указатель на POISON-элемент вектора
*   @param el_dtor         [in] - указатель на dtor элемента вектора
*   @param el_dump         [in] - указатель на dump элемента вектора
*   @param vector_capacity [in] - начальная емкость вектора
*
*   @return указатель на созданный вектор или nullptr в случае ошибки.
*/
inline vector *vector_new(const size_t el_size, const void *const el_poison                     = nullptr,
                                                      void (     *el_dtor  )(      void *const) = nullptr,
                                                      void (     *el_dump  )(const void *const) = nullptr,

                                                const size_t vector_capacity = DEFAULT_STACK_CAPACITY)
{
    return stack_new(el_size, el_poison, el_dtor, el_dump, vector_capacity);
}

/**
*   @brief Деструктор вектора.
*
*   @param _vec [in] - указатель на вектор
*/
inline void vector_dtor(void *const _vec)
{
    return stack_dtor(_vec);
}

/**
*   @brief Деструктор вектора в динамической памяти.
*
*   @see vector_dtor(void* _vec)
*/
inline void vector_delete(void *const _vec)
{
    return stack_delete(_vec);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Кладет элемент вектора в конец.
*
*   @param vec  [in, out] - указатель на вектор
*   @param data [in]      - указатель на элемент
*
*   @return true, в случае успеха, false в случае ошибки
*/
inline bool vector_push_back(vector *const vec, const void *const data)
{
    return stack_push(vec, data);
}

/**
*   @brief Удаляет последний элемент вектора.
*
*   @param vec  [in, out] - указатель на вектор
*   @param data [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
inline bool vector_pop_back(vector *const vec, void *const data = nullptr)
{
    return stack_pop(vec, data);
}

/**
*   @brief Возвращает указатель на элемент вектора.
*
*   @param vec   [in] - указатель на вектор
*   @param index [in] - индекс элемента вектора
*
*   @return true, в случае успеха, false в случае ошибки
*/
void *vector_get(const vector *const vec, const size_t index);

/**
*   @brief Возвращает указатель на начало вектора - его первый элемент или nullptr, если он пуст.
*/
void *vector_begin(const vector *const vec);

/**
*   @brief Возвращает указатель на конец вектора - следующий за его последним элементом или nullptr, если он пуст.
*/
void *vector_end(const vector *const vec);

/**
*   @brief Инициализирует элемент вектора.
*
*   @param vec   [in] - указатель на вектор
*   @param index [in] - индекс элемента вектора
*   @param data  [in] - указатель на содержимое, которым инциализировать элемент
*
*   @return true, в случае успеха, false в случае ошибки
*/
bool vector_set(vector *const vec, const size_t index, const void *const data);

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

/**
*   @brief Проверяет, пустой ли вектор.
*
*   @param vec [in] - указатель на вектор
*
*   @return true, если вектор пустой, false иначе, или в случае ошибки
*/
bool inline vector_is_empty(const vector *const vec)
{
    return stack_is_empty(vec);
}

/**
*   @brief vector_dump.
*
*   @param _vec [in] - указатель на вектор
*/
inline void vector_dump(const void *const _vec)
{
    return stack_dump(_vec);
}

#endif // VECTOR_H
