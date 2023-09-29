/** @file */
#ifndef STACK_H
#define STACK_H

#include <assert.h>
#include <stdlib.h>
#include "stack_settings.h"

//================================================================================================================================

/**
*   @brief Структура данных стек.
*/
struct stack
{
    void  *data;            ///< указатель на начало массива с элементами стека

    size_t  el_size;        ///< размер элемента стека (в байтах)
    size_t     size;        ///< размер стека (количество элементов в стеке)
    size_t capacity;        ///< емкость стека (максимальное количество элементов, которое может храниться в стеке)

    const void *el_poison;  ///< указатель на POISON-элемент стека

    void (*el_dtor) (      void *const el);    ///< указатель на dtor элемента стека
    void (*el_dump) (const void *const el);    ///< указатель на dump элемента стека
};

//================================================================================================================================

extern const size_t DEFAULT_STACK_CAPACITY;

//================================================================================================================================

/**
*   @brief Верификатор стека.
*   @return 0, если стек валидный.
*/
unsigned stack_verify(const stack *const stk);

/**
*   @brief Конструктор стека.
*
*   @param stk            [out] - указатель на стек
*   @param el_size        [in]  - размер элемента стека
*   @param el_poison      [in]  - указатель на POISON-элемент стека
*   @param el_dtor        [in]  - указатель на dtor элемента стека
*   @param el_dump        [in]  - указатель на dump элемента стека
*   @param stack_capacity [in]  - начальная емкость стека
*
*   @return true, если удалось создать стек, false - иначе.
*/
bool stack_ctor(stack *const stk, const size_t el_size, const void *const el_poison                     = nullptr,
                                                              void (     *el_dtor  )(      void *const) = nullptr,
                                                              void (     *el_dump  )(const void *const) = nullptr,

                                                              const size_t stack_capacity = DEFAULT_STACK_CAPACITY);

/**
*   @brief Создает стек в динамической памяти.
*
*   @param el_size        [in] - размер элемента стека
*   @param el_poison      [in] - указатель на POISON-элемент стека
*   @param el_dtor        [in] - указатель на dtor элемента стека
*   @param el_dump        [in] - указатель на dump элемента стека
*   @param stack_capacity [in] - начальная емкость стека
*
*   @return указатель на созданный стек или nullptr в случае ошибки.
*/
stack *stack_new(const size_t el_size, const void *const el_poison                     = nullptr,
                                             void (     *el_dtor  )(      void *const) = nullptr,
                                             void (     *el_dump  )(const void *const) = nullptr,

                                             const size_t stack_capacity = DEFAULT_STACK_CAPACITY);

/**
*   @brief Деструктор стека.
*
*   @param _stk [in] - указатель на стек
*/
void stack_dtor(void *const _stk);

/**
*   @brief Деструктор стека в динамической памяти.
*
*   @see stack_dtor(void* _stk)
*/
void stack_delete(void *const _stk);

/**
*   @brief Кладет элемент в конец стека.
*
*   @param stk  [in, out] - указатель на стек
*   @param data [in]      - указатель на элемент
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool stack_push(stack *const stk, const void *const data);

/**
*   @brief Удаляет последний элемент стека.
*
*   @param stk  [in, out] - указатель на стек
*   @param data [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool stack_pop(stack *const stk, void *const data = nullptr);

/**
*   @brief Показывает содержимое вершины стека.
*
*   @param stk  [in]  - указатель на стек
*   @param data [out] - указатель, куда скопировать содержимое удаляемого элемента
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool stack_front(const stack *const stk, void *const data);

/**
*   @brief Проверяет, пустой ли стек.
*
*   @param stk [in] - указатель на стек
*
*   @return true, если стек пустой, false иначе.
*/
bool stack_is_empty(const stack *const stk);

/**
*   @brief Дамп стека.
*
*   @param _stk [in] - указатель на стек
*/
void stack_dump(const void *const _stk);

//================================================================================================================================

#ifndef STACK_NVERIFY
#define STACK_VERIFY(stk, ret_val)  \
    if (stack_verify(stk) != 0)     \
    {                               \
        return ret_val;             \
    }
#else
#define STACK_VERIFY(stk, ret_val)
#endif

#ifndef STACK_NDEBUG
#define STACK_ASSERT(stk)                           \
    if (stack_verify(stk) != 0)                     \
    {                                               \
        fprintf(stderr, "STACK ASSERTION FAILED\n");\
        abort();                                    \
    }
#else
#define STACK_ASSERT(stk)
#endif

#endif // STACK_H
