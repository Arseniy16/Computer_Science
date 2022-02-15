#include <stdio.h>
#include "listPrivate.h"

err_t insert_before(List * list, Node * elem, const elem_t data) 
{
    if ((list == NULL) || (elem == NULL)) 
    {
        return INVALID_ARG;
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

err_t insertion_sort(List ** list, int (*cmp)(const void *, const void *))
{
    if ((*list) == NULL)
    {
        return INVALID_ARG;
    }

    List * out = list_create();
    Node * new = NULL;
    Node * old = NULL;

    list_pushFront(out, list_popFront(*list));

    old = (*list)->head;
    while (old)
    {
        new = out->head;
        while (new && (cmp((const void *) &(old->value), (const void *) &(new->value)) > 0) )
        {
            new = new->next;
        }
        if (new)
        {
            insert_before(out, new, old->value);
        }
        else 
        {
            list_pushBack(out, old->value);
        }

        old = old->next;     
    }

    free(*list);
    *list = out;

    return 0;
}
