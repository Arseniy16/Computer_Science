#include <stdio.h>
#include "list.h"

int ascending(const void * a, const void * b)
{ 
    return (*(int *)a) - (*(int *)b);
}

int descending(const void * a, const void * b)
{ 
    return (*(int *)b) - (*(int *)a);
}

int main()
{
    List * list = list_create();

    push_front(list, 10);
    push_front(list, 20);
    push_back(list, 50);
    push_back(list, 40);
    push_back(list, 50);
    push_back(list, 1);
    print_list(list);

#if 0    
    int a = pop_front(list); //20
    printf("a = %d \n", a);

    int b = pop_back(list); //50
    printf("b = %d \n", b);   

    print_list(list);
    insert(list, 1, 5);
    print_list(list);
    //printf("val = %d ", get_elem(list, 1)->value);
#endif

    insertion_sort(&list, ascending);
    print_list(list);

    insertion_sort(&list, descending);
    print_list(list);
    
    list_destroy(list);

    return 0;
}