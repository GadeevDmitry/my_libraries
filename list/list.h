/** @file */
#ifndef LIST_H
#define LIST_H

#include "../logs/log.h"
#include "../algorithm/algorithm.h"

//================================================================================================================================
// STRUCT
//================================================================================================================================

/**
*   @brief Вершина листа
*/
struct list_node
{
    void      *data;    ///< указатель на начало элемента листа
    list_node *prev;    ///< указатель на предыдущую вершину листа
    list_node *next;    ///< указатель на следующую вершину листа
};

/**
*   @brief Структура данных лист
*/
struct list
{
    list_node *fictional;                   ///< указатель на фиктивную вершину листа

    size_t el_size;                         ///< размер (в байтах) элемента листа
    size_t    size;                         ///< количество элементов в листе

    void (*el_dtor) (      void *const);    ///< указатель на dtor элемента листа
    void (*el_dump) (const void *const);    ///< указатель на dump элемента листа
};

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List_ctor.
*   Оболочка для back_trace.
*
*   @param cur_file [in]  - файл в точке вызова
*   @param cur_func [in]  - функция в точке вызова
*   @param cur_line [in]  - номер строки в точке вызова
*
*   @param lst      [out] - указатель на лист
*   @param el_size  [in]  - размер элемента листа
*   @param el_dtor  [in]  - указатель на dtor элемента листа
*   @param el_dump  [in]  - указатель на dump элемента листа
*/
bool _list_ctor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                list *const lst, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                       void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Создает лист в динамической памяти.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @param el_size  [in] - размер элемента листа
*   @param el_dtor  [in] - указатель на dtor элемента листа
*   @param el_dump  [in] - указатель на dump элемента листа
*
*   @return указатель на созданный лист или nullptr в случае ошибки
*/
list *_list_new(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                      void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List_dtor.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @param _lst     [in] - указатель на лист
*/
void _list_dtor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в лист.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param data     [in]      - указатель на элемент
*   @param index    [in]      - порядковый номер, на который поставить элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_insert(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  list *const lst, const void *const data, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в начало листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param data     [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_push_front(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Добавляет элемент в конец листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param data     [in]      - указатель на элемент
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_push_back(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param index    [in]      - порядковый номер удаляемого элемента
*   @param data     [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_erase(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 list *const lst, const size_t index, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из начала листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param data     [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_pop_front(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из конца листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]      - файл в точке вызова
*   @param cur_func [in]      - функция в точке вызова
*   @param cur_line [in]      - номер строки в точке вызова
*
*   @param lst      [in, out] - лист
*   @param data     [out]     - указатель, куда скопировать содержимое удаляемого элемента (nullptr по умолчанию)
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_pop_back(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое элемента листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]  - файл в точке вызова
*   @param cur_func [in]  - функция в точке вызова
*   @param cur_line [in]  - номер строки в точке вызова
*
*   @param lst      [in]  - лист
*   @param index    [in]  - порядковый номер элемента
*   @param data     [out] - указатель, куда скопировать содержимое элемента
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_get(const char *const cur_file,
               const char *const cur_func,
               const int         cur_line,

               const list *const lst, const size_t index, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое первого элемента листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]  - файл в точке вызова
*   @param cur_func [in]  - функция в точке вызова
*   @param cur_line [in]  - номер строки в точке вызова
*
*   @param lst      [in]  - лист
*   @param data     [out] - указатель, куда скопировать содержимое элемента
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_front(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Показывает содержимое последнего элемента листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in]  - файл в точке вызова
*   @param cur_func [in]  - функция в точке вызова
*   @param cur_line [in]  - номер строки в точке вызова
*
*   @param lst      [in]  - лист
*   @param data     [out] - указатель, куда скопировать содержимое элемента
*
*   @return true, если все ОК, false в случае ошибки
*/
bool _list_back(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Дамп листа.
*   Оболочка для back_trace.
*
*   @param cur_file [in] - файл в точке вызова
*   @param cur_func [in] - функция в точке вызова
*   @param cur_line [in] - номер строки в точке вызова
*
*   @param lst      [in] - указатель на лист
*/
void _list_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const void *const _lst);

#include "list_def.h"

#endif //LIST_H