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

bool _list_ctor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                list *const lst, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                       void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

list *_list_new(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                      void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

void _list_dtor(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                void *const _lst);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

bool _list_insert(const char *const cur_file,
                  const char *const cur_func,
                  const int         cur_line,

                  list *const lst, const void *const data, const size_t index);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_push_front(const char *const cur_file,
                      const char *const cur_func,
                      const int         cur_line,

                      list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_push_back(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, const void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_erase(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 list *const lst, const size_t index, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_pop_front(const char *const cur_file,
                     const char *const cur_func,
                     const int         cur_line,

                     list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_pop_back(const char *const cur_file,
                    const char *const cur_func,
                    const int         cur_line,

                    list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

bool _list_get(const char *const cur_file,
               const char *const cur_func,
               const int         cur_line,

               const list *const lst, const size_t index, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_front(const char *const cur_file,
                 const char *const cur_func,
                 const int         cur_line,

                 const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------

bool _list_back(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void _list_dump(const char *const cur_file,
                const char *const cur_func,
                const int         cur_line,

                const void *const _lst);

#include "list_def.h"

#endif //LIST_H