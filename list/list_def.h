#ifndef LIST_DEF_H
#define LIST_DEF_H

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

#define list_ctor(lst, el_size, ...) _list_ctor(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, el_size, ##__VA_ARGS__)
#define list_new(      el_size, ...) _list_new (__FILE__, __PRETTY_FUNCTION__, __LINE__,      el_size, ##__VA_ARGS__)

//--------------------------------------------------------------------------------------------------------------------------------
// dtor
//--------------------------------------------------------------------------------------------------------------------------------

#define list_dtor(_lst) _list_dtor(__FILE__, __PRETTY_FUNCTION__, __LINE__, _lst)

//--------------------------------------------------------------------------------------------------------------------------------
// insert erase
//--------------------------------------------------------------------------------------------------------------------------------

#define list_insert(    lst, data, index) _list_insert    (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, data, index)
#define list_push_front(lst, data)        _list_push_front(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, data       )
#define list_push_back( lst, data)        _list_push_back (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, data       )

#define list_erase(    lst, index, ...)   _list_erase     (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, index, ##__VA_ARGS__)
#define list_pop_front(lst,        ...)   _list_pop_front (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst,        ##__VA_ARGS__)
#define list_pop_back( lst,        ...)   _list_pop_back  (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst,        ##__VA_ARGS__)

//--------------------------------------------------------------------------------------------------------------------------------
// list get
//--------------------------------------------------------------------------------------------------------------------------------

#define list_get(  lst, index, data) _list_get  (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst, index, data)
#define list_front(lst,        data) _list_front(__FILE__, __PRETTY_FUNCTION__, __LINE__, lst,        data)
#define list_back( lst,        data) _list_back (__FILE__, __PRETTY_FUNCTION__, __LINE__, lst,        data)

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

#define list_dump(_lst) _list_dump(__FILE__, __PRETTY_FUNCTION__, __LINE__, _lst)

#endif //LIST_DEF_H