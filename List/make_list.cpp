#include "list.hpp"
#include <stdio.h>
#include <assert.h>


static int make_list_base(          struct list **list_ptr);
static int list_make_data_struct(   struct list *list);
static int list_to_base_values(     struct list *list);



int make_list(struct list **list_ptr, size_t size) {
    assert(list_ptr != NULL);

    int err = LIST_OK;

    if ((err = make_list_base(list_ptr)) != LIST_OK) {
        list_dump(*list_ptr, err);
        return err;
    }

    SIZE(*list_ptr) = size;

    FREE(*list_ptr) = 1;

    if ((err = list_make_data_struct(*list_ptr)) != LIST_OK) {
        list_dump(*list_ptr, err);
        return err;
    }

    list_error(*list_ptr);

    return LIST_OK;
}


int make_list_base(struct list **list_ptr) {
    assert(list_ptr != NULL);

    struct list *list = (struct list *) malloc(sizeof(struct list));

    if (list == NULL) {
        return LIST_ALLOC_FAILED;
    }

    *list_ptr = list;

    return LIST_OK;
}


int list_make_data_struct(struct list *list) {
    assert(list != NULL);

    DATA(list) = (list_elem *) calloc(SIZE(list), sizeof(list_elem));
    if (DATA(list) == NULL) {
        return LIST_DATA_ALLOC_FAILED;
    }

    list_to_base_values(list);

    return LIST_OK;
}


int list_to_base_values(struct list *list) {
    assert(list != NULL);

    VALUE(list, 0) = LIST_POISON;
    TAIL(list) = 0;
    HEAD(list) = 0;

    for (size_t idx = 1; idx < SIZE(list); ++idx) {
        DATA(list)[idx] = LIST_BASE_VALUE;
        PREV(list, idx) = (idx + 1);
    }
    PREV(list, SIZE(list) - 1) = 0;

    return LIST_OK;
}


int list_free(struct list *list) {
    assert(list != NULL);

    free(DATA(list));
    free(list);

    return LIST_OK;
}
