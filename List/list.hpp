#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define MY_RED "\033[31m"
#define BASE_CLR "\033[0m"

#define LIST_NO_NEXT -1
#define LIST_NO_PREV -2
#define LIST_BASE_VALUE {LIST_POISON, LIST_NO_NEXT, LIST_NO_PREV}
#define LIST_POISON -67

#define CHECK_ERR(...) { int err = 0; if ((err = __VA_ARGS__) != LIST_OK) return err; }

typedef int list_elem_t;


enum list_error {
    LIST_OK = 0,
    LIST_ALLOC_FAILED,
    LIST_DATA_ALLOC_FAILED,
    LIST_NULL_PTR,
    LIST_DATA_NULL_PTR,
    LIST_INVALID_TAIL,
    LIST_INVALID_HEAD,
    LIST_INVALID_FIRST_FREE,
    GLIST,
};


struct list_elem {
    list_elem_t value = 0;
    short next_idx = 0;
    short prev_idx = 0;
};

struct list {
    struct list_elem *elems = NULL;

    short tail = 0;
    short head = 0;
    short free_idx = 0;

    size_t size = 0;
};


int make_list(struct list **list_ptr, size_t size);
int list_free(struct list *list);

void list_dump(struct list *list);

int list_check(struct list *list);

bool list_is_last(struct list *list, short idx);
int list_add(struct list *list, int position, list_elem_t value);

#endif
