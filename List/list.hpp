#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define MY_RED "\033[31m"
#define BASE_CLR "\033[0m"

#define LIST_NO_NEXT 7676
#define LIST_NO_PREV 6767
#define LIST_BASE_VALUE {LIST_POISON, LIST_NO_NEXT, LIST_NO_PREV}
#define LIST_POISON -67


#define DATA(list) (list)->elems
#define SIZE(list) (list)->size

#define HEAD(list) (list)->elems[0].prev_idx
#define TAIL(list) (list)->elems[0].next_idx
#define FREE(list) (list)->free_idx

#define NEXT(list, num)  (list)->elems[(num)].next_idx
#define PREV(list, num)  (list)->elems[(num)].prev_idx
#define VALUE(list, num) (list)->elems[(num)].value


#define EXIT_ERR(...) { int err = 0; if ((err = __VA_ARGS__) != LIST_OK) exit(err); }
#define CHECK_ERR(...) { int err = 0; if ((err = __VA_ARGS__) != LIST_OK) return err; }

typedef ssize_t list_elem_t;



#define    LIST_OK                  0
#define    LIST_ALLOC_FAILED        1
#define    LIST_DATA_ALLOC_FAILED   2
#define    LIST_NULL_PTR            3
#define    LIST_DATA_NULL_PTR       4
#define    LIST_INVALID_TAIL        5
#define    LIST_INVALID_HEAD        6
#define    LIST_INVALID_FIRST_FREE  7
#define    LIST_UNDERFLOW           8
#define    LIST_OVERFLOW            9
#define    LIST_INVALID_TERMINATOR  10
#define    LIST_CANT_PUSH           11
#define    LIST_CANT_POP            12



struct list_elem {
    list_elem_t value = 0;
    size_t next_idx = 0; // TODO: to size_t
    size_t prev_idx = 0;
};

struct list {
    struct list_elem *elems = NULL;

    size_t free_idx = 0;

    size_t size = 0;
};

//TODO: swap 0th next and prev
int make_list(struct list **list_ptr, size_t size);
int list_free(struct list *list);

void list_dump(struct list *list, int error);
int list_error(struct list *list);

int list_dump_to_graphviz(struct list *list);

int list_check(struct list *list);

bool list_is_last(  struct list *list, size_t idx);
bool list_is_free_element(struct list *list, size_t idx);
int list_push_back( struct list *list, list_elem_t value);
int list_pop_back(  struct list *list, list_elem_t *value);
int list_push(      struct list *list, size_t position, list_elem_t value);
int list_pop(       struct list *list, size_t position, list_elem_t *value);



#endif
