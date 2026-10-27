#include "list.hpp"
#include <stdio.h>
#include <assert.h>


static int make_list_base(          struct list **list_ptr);
static int list_make_data_struct(   struct list *list);
static int list_to_base_values(     struct list *list);



int make_list(struct list **list_ptr, size_t size) {
    assert(list_ptr != NULL);

    make_list_base(list_ptr);

    (*list_ptr)->size = size;
    (*list_ptr)->head = 0;
    (*list_ptr)->tail = 0;
    (*list_ptr)->free_idx = 0;

    list_make_data_struct(*list_ptr);

    return LIST_OK;
}


int make_list_base(struct list **list_ptr) {
    assert(list_ptr != NULL);

    struct list *list = (struct list *) malloc(sizeof(struct list));

    if (list == NULL) {
        fprintf(stderr, "Error: failed to allocate list\n");
        return LIST_ALLOC_FAILED;
    }

    *list_ptr = list;

    return LIST_OK;
}


int list_make_data_struct(struct list *list) {
    assert(list != NULL);

    list->elems = (list_elem *) calloc(list->size, sizeof(list_elem));
    if (list->elems == NULL) {
        fprintf(stderr, "Error: failed to allocate data array\n");
        return LIST_DATA_ALLOC_FAILED;
    }

    list_to_base_values(list);

    return LIST_OK;
}


int list_to_base_values(struct list *list) {
    assert(list != NULL);

    list->elems[0].value = LIST_POISON;
    list->elems[0].next_idx = 0;
    list->elems[0].prev_idx = 0;

    for (size_t idx = 1; idx < list->size; ++idx) {
        list->elems[idx] = LIST_BASE_VALUE;
    }

    return LIST_OK;
}


int list_free(struct list *list) {
    assert(list != NULL);

    free(list->elems);
    free(list);

    return LIST_OK;
}
