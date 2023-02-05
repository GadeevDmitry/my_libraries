/** @file */
#ifndef STACK_H
#define STACK_H

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Структура данных стек
*/
struct stack
{
    void  *data;            ///< указатель на начало массива с элементами стека

    size_t  el_size;        ///< размер элемента стека (в байтах)
    size_t     size;        ///< размер стека (количество элементов в стеке)
    size_t capacity;        ///< емкость стека (максимальное количество элементов, которое может храниться в стеке)

    const void *el_poison;  ///< указатель на POISON-элемент стека

    void (*el_dtor)      (      void *const el);    ///< указатель на dtor элемента стека
    void (*el_dump)      (const void *const el);    ///< указатель на dump элемента стека
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack_ctor.
*   Оболочка для back trace.
*
*   @param cur_file  [in] - файл в точке вызова
*   @param cur_func  [in] - функция в точке вызова
*   @param cur_line  [in] - строка в точке вызова
*
*   @param stk       [out] - указатель на стек
*   @param el_size   [in]  - размер элемента стека
*   @param el_poison [in]  - указатель на POISON-элемент стека
*   @param el_dtor   [in]  - указатель на dtor элемента стека
*   @param el_dump   [in]  - указатель на dump элемента стека
*/
bool _stack_ctor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const size_t el_size, const void *const el_poison                     = nullptr,
                                                               void (     *el_dtor  )(      void *const) = nullptr,
                                                               void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает стек в динамической памяти.
*   Оболочка для back trace.
*
*   @param cur_file  [in] - файл в точке вызова
*   @param cur_func  [in] - функция в точке вызова
*   @param cur_line  [in] - строка в точке вызова
*
*   @param el_size   [in]  - размер элемента стека
*   @param el_poison [in]  - указатель на POISON-элемент стека
*   @param el_dtor   [in]  - указатель на dtor элемента стека
*   @param el_dump   [in]  - указатель на dump элемента стека
*
*   @return указатель на созданный стек или nullptr в случае ошибки
*/
stack *_stack_new(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const size_t el_size, const void *const el_poison                     = nullptr,
                                              void (     *el_dtor  )(      void *const) = nullptr,
                                              void (     *el_dump  )(const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack_dtor.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param stk      [in] - указатель на стек
*/
void _stack_dtor(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 void *const _stk);

//--------------------------------------------------------------------------------------------------------------------------------
// push pop
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Кладет элемент стека в конец.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - строка в точке вызова
*
*   @param stk      [in, out] - стек
*   @param data     [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _stack_push(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 stack *const stk, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет последний элемент стека.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - строка в точке вызова
*
*   @param stk      [in, out] - стек
*   @param data     [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _stack_pop(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                stack *const stk, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// other
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое последнего элемента стека.
*   Оболочка для back_trace.
*
*   @param cur_file [in]  - файл в точке вызова
*   @param cur_func [in]  - функция в точке вызова
*   @param cur_line [in]  - строка в точке вызова
*
*   @param stk      [in]  - стек
*   @param data     [out] - указатель, куда скопировать содержимое удаляемого элемента
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _stack_front(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  const stack *const stk, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Проверяет, пустой ли стек.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param stk      [in] - стек
*
*   @return true, если стек пустой, false иначе, или в случае ошибки
*/
bool _stack_is_empty(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     const stack *const stk);

//--------------------------------------------------------------------------------------------------------------------------------
// stack dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Stack_dump.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - строка в точке вызова
*
*   @param _stk     [in] - стек
*/
void _stack_dump(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const void *const _stk);

#include "stack_def.h"

#endif //STACK_H