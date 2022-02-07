#include <stdio.h>
#include "list.h"

void insert_before(List * list, Node * elem, const int data) 
{
    Node * new = NULL;
    if (elem == NULL) 
    {
        return;
    }
     
    if (elem->prev == NULL) 
    {
        push_front(list, data);
        return;
    }

    new = (Node*) malloc(sizeof(Node));
    new->value = data;
    new->prev = elem->prev;
    elem->prev->next = new;
    new->next = elem;
    elem->prev = new;
 
    list->size++;
}

void insertion_sort(List ** list, int (*cmp)(const void *, const void *))
{
    List * out = list_create();
    Node * new = NULL;
    Node * old = NULL;

    push_front(out, pop_front(*list));

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
            push_back(out, old->value);
        }

        old = old->next;     
    }

    free(*list);
    *list = out;
}
