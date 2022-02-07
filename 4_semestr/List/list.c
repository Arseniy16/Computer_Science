#include <stdio.h>
#include "list.h"

List * list_create()
{
    List * list = (List *) malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->size = 0;

    return list;
}

List * list_init(const size_t len, const int * data)
{
    List * list = (List *) malloc(sizeof(List));

    list->head = (Node *) malloc(sizeof(Node));
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

void list_destroy(List * list)
{
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
}

void push_back(List * list, const int data)
{
    Node * tmp = (Node *) malloc(sizeof(Node));

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
}

int pop_back(List * list)
{
    if (list->tail == NULL)
    {
        return -1;
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

    int tmp = next->value;
    
    free(next);
    
    list->size--;
    return tmp;
}

void push_front(List * list, const int data)
{
    Node * tmp = (Node *) malloc(sizeof(Node));

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
}

int pop_front(List * list)
{
    if (list->head == NULL) 
    {
        return -1;
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

    int tmp = prev->value;
    free(prev);
 
    list->size--;
    return tmp;
}

Node * get_elem(List * list, const size_t index)
{
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

void insert(List * list, const size_t index, const int data)
{
    Node * elem = get_elem(list, index);
    if (elem == NULL)
    {
        return;
    }

    Node * new = (Node *) malloc(sizeof(Node));
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
}

void print_list(List * list)
{
    Node * tmp = list->head;

    while (tmp)
    {
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}