#include <stdio.h>
#include "listPrivate.h"
//#include "list.h"

List * list_create()
{
    List * list = (List *) malloc(sizeof(List));
    if (list == NULL)
    {
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    return list;
}

List * list_init(const size_t len, const elem_t * data)
{
    List * list = (List *) malloc(sizeof(List));
    if (list == NULL)
    {
        return NULL;
    }
    
    list->head = (Node *) malloc(sizeof(Node));
    if (list->head == NULL)
    {
        free(list);
        return NULL;
    }

    Node * current = list->head;

    for (size_t i = 0; i < len; ++i)
    {
        current->value = data[i];
        if (i < (len - 1))
        {
            current->next = (Node *) malloc(sizeof(Node));
            current = current->next;
        }
    }

    list->tail = current;
    list->size = len;

    return list;
}

err_t list_destroy(List * list)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * tmp = list->head;
    Node * next = NULL;

    while (tmp) 
    {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }

    free(list);
    list = NULL;

    return 0;
}

err_t list_pushBack(List * list, const elem_t data)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * tmp = (Node *) malloc(sizeof(Node));
    if (tmp == NULL)
    {
        return MEM_ERR;
    }

    tmp->value = data;
    tmp->next = NULL;
    tmp->prev = list->tail;

    if (list->tail)
    {
        list->tail->next = tmp;
    }
    
    list->tail = tmp;

    if (list->head == NULL)
    {
        list->head = tmp;
    }

    list->size++;

    return 0;
}

elem_t list_popBack(List * list)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }
    if (list->tail == NULL)
    {
        return LIST_EMPTY;
    }

    Node * next = list->tail;
    list->tail = list->tail->prev;

    if (list->tail) 
    {
        list->tail->next = NULL;
    }
    if (next == list->head) 
    {
        list->head = NULL;
    }

    elem_t tmp = next->value;
    
    free(next);
    
    list->size--;
    return tmp;
}

err_t list_pushFront(List * list, const elem_t data)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * tmp = (Node *) malloc(sizeof(Node));
    if (tmp == NULL)
    {
        return MEM_ERR;
    }

    tmp->value = data;
    tmp->next = list->head;
    tmp->prev = NULL;

    if (list->head) 
    {
        list->head->prev = tmp;
    }

    list->head = tmp;
 
    if (list->tail == NULL) 
    {
        list->tail = tmp;
    }

    list->size++;

    return 0;
}

elem_t list_popFront(List * list)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }
    if (list->head == NULL) 
    {
        return LIST_EMPTY;
    }
 
    Node * prev = list->head;
    list->head = list->head->next;
    
    if (list->head) 
    {
        list->head->prev = NULL;
    }

    if (prev == list->tail) 
    {
        list->tail = NULL;
    }

    elem_t tmp = prev->value;
    free(prev);
 
    list->size--;
    return tmp;
}

Node * list_getPtr(List * list, const size_t index)
{
    if (list == NULL)
    {
        return NULL;
    }

    Node * tmp = NULL;

    if (index < list->size/2)
    {
        tmp = list->head;
        for (size_t i = 0; tmp && (i < index); ++i)
        {
            tmp = tmp->next;
        }
    }
    else 
    {
        tmp = list->tail;
        for (size_t i = list->size-1; tmp && (i > index); --i)
        {
            tmp = tmp->prev;
        }
    }
    
    return tmp;
}

elem_t list_getElem(List * list, const size_t index)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * node = list_getPtr(list, index);
    if (node == NULL)
    {
        return NO_ELEM;
    }

    return node->value;
} 

err_t list_insertAfter(List * list, const size_t index, const elem_t data)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * elem = list_getPtr(list, index);

    Node * new = (Node *) malloc(sizeof(Node));
    if (new == NULL)
    {
        return MEM_ERR;
    }

    new->value = data;
    new->prev = elem;
    new->next = elem->next; 

    if (elem->next)
    {
        elem->next->prev = new;
    }
    
    elem->next = new;

    if (elem->prev == NULL)
    {
        list->head = elem;
    }
    if (elem->next == NULL)
    {
        list->tail = elem;
    }

    list->size++;

    return 0;
}

err_t list_insertBefore(List * list, const size_t index, const elem_t data) 
{
    if (list == NULL) 
    {
        return INVALID_ARG;
    }

    Node * elem = list_getPtr(list, index);
    if (elem == NULL)
    {   
        return LIST_EMPTY;
    }

    Node * new = NULL;     
    if (elem->prev == NULL) 
    {
        list_pushFront(list, data);
        return 0;
    }

    new = (Node*) malloc(sizeof(Node));
    if (new == NULL)
    {
        return MEM_ERR;
    }

    new->value = data;
    new->prev = elem->prev;
    elem->prev->next = new;
    new->next = elem;
    elem->prev = new;
 
    list->size++;

    return 0;
}

err_t list_print(List * list)
{
    if (list == NULL)
    {
        return INVALID_ARG;
    }

    Node * tmp = list->head;

    while (tmp)
    {
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");

    return 0;
}