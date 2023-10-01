/** @file */
#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>
#include "list_settings.h"

//================================================================================================================================

/**
*   @brief Вершина листа.
*/
struct list_node
{
    list_node  *prev;   ///< указатель на предыдущую вершину листа
    list_node  *next;   ///< указатель на следующую вершину листа
};

/**
*   @brief Структура данных лист.
*/
struct list
{
    list_node *fictional;                   ///< указатель на фиктивную вершину листа

    size_t    size;                         ///< количество элементов в листе
    size_t el_size;                         ///< размер элемента листа

    void (*el_dtor) (      void *const);    ///< указатель на dtor элемента листа
    void (*el_dump) (const void *const);    ///< указатель на dump элемента листа
};

//================================================================================================================================

/**
*   @brief Верификатор листа.
*   @return 0, если лист валидный.
*/
unsigned list_verify(const list *const lst);

/**
*   @brief Конструктор листа.
*
*   @param lst     [out] - указатель на лист
*   @param el_size [in]  - размер элемента листа (в байтах)
*   @param el_dtor [in]  - указатель на dtor элемента листа
*   @param el_dump [in]  - указатель на dump элемента листа
*
*   @return true, если удалось создать лист, false - иначе.
*/
bool list_ctor(list *const lst, const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                                      void (*el_dump) (const void *const) = nullptr);

/**
*   @brief Создает лист в динамической памяти.
*
*   @param el_size [in] - размер элемента листа (в байтах)
*   @param el_dtor [in] - указатель на dtor элемента листа
*   @param el_dump [in] - указатель на dump элемента листа
*
*   @return указатель на созданный лист или nullptr в случае ошибки.
*/
list *list_new(const size_t el_size, void (*el_dtor) (      void *const) = nullptr,
                                     void (*el_dump) (const void *const) = nullptr);

/**
*   @brief Деструктор листа.
*
*   @param _lst [in] - указатель на лист
*/
void list_dtor(void *const _lst);

/**
*   @brief Деструктор листа в динамической памяти.
*
*   @see list_dtor(void* _lst)
*/
void list_delete(void *const _lst);

/**
*   @brief Кладет элемент в лист.
*
*   @param lst   [in, out] - указатель на лист
*   @param data  [in]      - указатель на элемент
*   @param index [in]      - порядковый номер, на который поставить элемент
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_insert(list *const lst, const void *const data, const size_t index);

/**
*   @brief Кладет элемент в начало листа.
*
*   @param lst  [in, out] - указатель на лист
*   @param data [in]      - указатель на элемент
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_push_front(list *const lst, const void *const data);

/**
*   @brief Кладет элемент в конец листа.
*
*   @param lst  [in, out] - указатель на лист
*   @param data [in]      - указатель на элемент
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_push_back(list *const lst, const void *const data);

/**
*   @brief Удаляет элемент из листа.
*
*   @param lst     [in, out] - указатель на лист
*   @param pos         [in]  - порядковый номер удаляемого элемента
*   @param erased_data [out] - указатель, по которому скопировать содержимое удаляемой вершины (nullptr по умолчанию)
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_erase(list *const lst, const size_t pos, void *const erased_data = nullptr);

//--------------------------------------------------------------------------------------------------------------------------------

/**
*   @brief Удаляет элемент из начала листа.
*
*   @param lst     [in, out] - указатель на лист
*   @param erased_data [out] - указатель, по которому скопировать содержимое удаляемой вершины (nullptr по умолчанию)
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_pop_front(list *const lst, void *const erased_data = nullptr);

/**
*   @brief Удаляет элемент из конца листа.
*
*   @param lst     [in, out] - указатель на лист
*   @param erased_data [out] - указатель, по которому скопировать содержимое удаляемой вершины (nullptr по умолчанию)
*
*   @return true, в случае успеха, false в случае ошибки.
*/
bool list_pop_back(list *const lst, void *const erased_data = nullptr);

/**
*   @brief Показывает содержимое элемента листа.
*
*   @param lst [in] - указатель на лист
*   @param pos [in] - порядковый номер элемента
*
*   @return указатель на содержимое вершины, в случае успеха, nullptr в случае ошибки.
*/
void *list_get(const list *const lst, const size_t pos);

/**
*   @brief Показывает содержимое первого элемента листа.
*
*   @param lst [in] - указатель на лист
*
*   @return указатель на содержимое вершины, в случае успеха, nullptr в случае ошибки.
*/
void *list_front(const list *const lst);

/**
*   @brief Показывает содержимое последнего элемента листа.
*
*   @param lst  [in]  - указатель на лист
*   @param data [out] - указатель, куда скопировать содержимое элемента
*
*   @return указатель на содержимое вершины, если все ОК, nullptr в случае ошибки.
*/
void *list_back(const list *const lst);

/**
*   @brief Возвращает указатель на фиктивный элемент листа.
*   Фиктивный элемент - элемент, следующий за последним и идущий перед первым.
*   Указатель на фиктивный элемент можно использовать для проверки выхода за границы листа в цикле.
*
*   @param lst [in] - указатель на лист
*/
void *list_fict(const list *const lst);

/**
*   @brief Ищет элемент в листе.
*
*   @param lst      [in] - указатель на лист
*   @param target   [in] - указатель на элемент, который нужно найти
*   @param elem_cmp [in] - указатель на функцию сравнения элементов, которая возвращает 0, если элементы равны
*
*   @return указатель на первое вхождение найденного элемента или nullptr, если его в листе нет.
*/
void *list_find(const list *const lst, const void *const target, int (*el_cmp)(const void *el_1, const void *el_2));

/**
*   @brief Возвращает указатель на элемент, следующий за текущим.
*   Ответственность за валидность переданного указателя и соблюдение границ листа несет пользователь.
*
*   @param cur_el [in] - указатель на текущий элемент листа
*
*   @return указатель на следующий элемент листа.
*/
void *list_next(const void *cur_el);

/**
*   @brief Возвращает указатель на предыдущий элемент.
*   Ответственность за валидность переданного указателя и соблюдение границ листа несет пользователь.
*
*   @param cur_el [in] - указатель на текущий элемент листа
*
*   @return указатель на следующий элемент листа.
*/
void *list_prev(const void *cur_el);

/**
*   @brief Переставляет элемент с одной позиции на другую.
*
*   @param lst [in, out] - указатель на лист
*   @param src_pos  [in] - порядковый номер элемента, которого нужно переместить
*   @param dest_pos [in] - номер, на который нужно переместить элемент
*
*   @return true в случае успеха, false в случае ошибки
*/
bool list_replace(const list *const lst, const size_t src_pos, const size_t dest_pos);

/**
*   @brief Переставляет элемент с одной позиции на другую.
*
*   @param lst [in, out] - указатель на лист
*   @param src_el   [in] - указатель на элемент, который нужно перемтавить
*   @param src_pos  [in] - порядковый номер элемента, которого нужно переместить
*   @param dest_pos [in] - номер, на который нужно переместить элемент
*
*   @return true в случае успеха, false в случае ошибки
*/
bool list_replace(const list *const lst, const void *src_el, const size_t src_pos, const size_t dest_pos);

/**
*   @brief Дамп листа.
*
*   @param lst [in] - указатель на лист
*/
void list_dump(const void *const _lst);

//================================================================================================================================

#ifndef LIST_NVERIFY
#define LIST_VERIFY(lst, ret_val)   \
    if (list_verify(lst) != 0)      \
    {                               \
    $o  return ret_val;             \
    }
#else
#define LIST_VERIFY(lst, ret_val)
#endif

#ifndef LIST_NDEBUG
#define LIST_ASSERT(lst)                            \
    if (list_verify(lst) != 0)                      \
    {                                               \
        fprintf(stderr, "LIST ASSERTION FAILED\n"); \
        abort();                                    \
    }
#else
#define LIST_ASSERT(lst)
#endif

#endif // LIST_H
