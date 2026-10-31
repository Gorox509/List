#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define MY_RED "\033[31m"
#define BASE_CLR "\033[0m"

#define LIST_NO_NEXT 7676
#define LIST_NO_PREV 6767
#define LIST_BASE_VALUE {LIST_POISON, LIST_NO_NEXT, LIST_NO_PREV}
#define LIST_POISON -67

#define CHECK_ERR(...) { int err = 0; if ((err = __VA_ARGS__) != LIST_OK) return err; }

typedef ssize_t list_elem_t;


enum list_error { // TODO: to define
    LIST_OK = 0,
    LIST_ALLOC_FAILED,
    LIST_DATA_ALLOC_FAILED,
    LIST_NULL_PTR,
    LIST_DATA_NULL_PTR,
    LIST_INVALID_TAIL,
    LIST_INVALID_HEAD,
    LIST_INVALID_FIRST_FREE,
    LIST_UNDERFLOW,
    LIST_OVERFLOW,
    LIST_INVALID_TERMINATOR,
    GLIST,
};


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

int list_check(struct list *list);

bool list_is_last(  struct list *list, size_t idx);
int list_push_back( struct list *list, list_elem_t value);
int list_pop_back(  struct list *list, list_elem_t *value);
int list_push(      struct list *list, size_t position, list_elem_t value);

#endif
