/** @file */
#ifndef LIST_H
#define LIST_H

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
*   @brief List ctor
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                      void (*el_dump) (const void *const) = nullptr);

/**
*   @brief Создаёт лист в динамической памяти
*
*   @return указаталь на созданный лист, nullptr в случае ошибки
*/
list *list_new(const size_t el_size,    void (*el_dtor) (      void *const) = nullptr,
                                        void (*el_dump) (const void *const) = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List dtor
*/
void list_dtor(void *const lst);

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List insert
*
*   @param lst   [in] - list to insert in
*   @param data  [in] - указатель на элемент листа, который нужно добавить в лист
*   @param index [in] - позиция, в которую добавить элемент листа
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool list_insert(list *const lst, const void *const data, const size_t index);

/**
*   @brief List push front
*   = list_insert(lst, data, 0)
*
*   @see bool list_insert(list *const lst, const void *const data, const size_t index)
*/
bool list_push_front(list *const lst, const void *const data);

/**
*   @brief List push back
*   = list_insert(lst, data, <размер листа>)
*
*   @see bool list_insert(list *const lst, const void *const data, const size_t index)
*/
bool list_push_back(list *const lst, const void *const data);

/**
*   @brief List erase
*
*   @param lst   [in]  - list to erase from
*   @param index [in]  - номер элемента, который нужно удалить из листа
*   @param data  [out] - указатель на начало массива, куда скопировать данные удаляемого элемента
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool list_erase(list *const lst, const size_t index, void *const data = nullptr);

/**
*   @brief List pop front
*   = list_erase(lst, 0, data)
*
*   @see bool list_erase(list *const lst, const size_t index, void *const data)
*/
bool list_pop_front(list *const lst, void *const data = nullptr);

/**
*   @brief List pop back
*   = list_erase(lst, <размер листа - 1>, data)
*
*   @see bool list_erase(list *const lst, const size_t index, void *const data)
*/
bool list_pop_back(list *const lst, void *const data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List get
*
*   @param lst   [in]  - list to get from
*   @param index [in]  - индекс интересующего элемента
*   @param data  [out] - указатель на начало массива, куда скопировать данные интересующего элемента
*
*   @return true, если всё ОК, false в случае ошибки
*/
bool list_get(const list *const lst, const size_t index, void *const data);

/**
*   @brief List front
*   = list_get(lst, 0, data)
*
*   @see bool list_get(const list *const lst, const size_t index, void *const data)
*/
bool list_front(const list *const lst, void *const data);

/**
*   @brief List back
*   = list_get(lst, <размер листа - 1>, data)
*
*   @see bool list_get(const list *const lst, const size_t index, void *const data)
*/
bool list_back(const list *const lst, void *const data);

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief List dump
*/
void list_dump(const void *const _lst);

#endif //LIST_H