#pragma once

#include <stdio.h>
#include <stdlib.h>

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
List * list_init(const size_t len, const int * data);
void list_destroy(List * list);

void push_back(List * list, const int data);
int pop_back(List * list);
void push_front(List * list, const int data);
int pop_front(List * list);
Node * get_elem(List * list, const size_t index);
void insert(List * list, const size_t index, const int data);
void print_list(List * list);

// for sort_list
void insert_before(List * list, Node * elem, const int data);
void insertion_sort(List ** list, int (*cmp)(const void *, const void *));
