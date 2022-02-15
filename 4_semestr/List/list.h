#ifndef _LIST_H_
#define _LIST_H_

enum Errors
{
    INVALID_ARG = -1000,
    MEM_ERR,
    LIST_EMPTY,
    NO_ELEM;
};


// for sort_list

err_t insert_before(List * list, Node * elem, const elem_t data);
err_t insertion_sort(List ** list, int (*cmp)(const void *, const void *));

#endif //_LIST_H_