#ifndef _LIST_PRIVATE_H_
#define _LIST_PRIVATE_H_

#include "list.h"
//#include "container.h"

typedef struct Node
{
    int value;
    struct Node * next;
    struct Node * prev; 
} Node;

typedef struct List
{
    size_t size;
    Node * head;
    Node * tail;
} List;

// for list
List * list_create();
List * list_init(const size_t len, const elem_t * data);
err_t list_destroy(List * list);

err_t list_pushBack(List * list, const elem_t data);
elem_t list_popBack(List * list);
err_t list_pushFront(List * list, const elem_t data);
elem_t list_popFront(List * list);
Node * list_getPtr(List * list, const size_t index);
elem_t list_getElem(List * list, const size_t index);
err_t list_insertAfter(List * list, const size_t index, const elem_t data);
//todo
err_t list_insertBefore(List * list, Node * elem, const elem_t data);
err_t list_print(List * list);


#endif //_LIST_PRIVATE_H_