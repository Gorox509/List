#include "list.hpp"
#include <assert.h>


int list_error(struct list *list) {
    int err = LIST_OK;

    if ((err = list_check(list)) != LIST_OK) {
        list_dump(list);
        return err;
    }

    return LIST_OK;
}


int list_check(struct list *list) { // TODO: to dump
    if (list == NULL)
        return LIST_NULL_PTR;
    if (list->elems == NULL)
        return LIST_DATA_NULL_PTR;
    if (list->size < (size_t) list->tail || list->tail < 0)
        return LIST_INVALID_TAIL;
    if (list->size < (size_t) list->head || list->head < 0)
        return LIST_INVALID_HEAD;
    if (list->size < (size_t) list->free_idx || list->free_idx < 0)
        return LIST_INVALID_FIRST_FREE;

    return LIST_OK;
}


bool list_is_last(struct list *list, short idx) {
    list_error(list);

    if (list->elems[idx].next_idx == 0) return true;
    return false;
}


int list_push(struct list *list, list_elem_t value) {
    list_error(list);

    list->elems[list->head].next_idx = list->free_idx;
    list->head = list->free_idx;
    list->free_idx = list->free_idx + 1; // TODO: redo

    list->elems[list->head].value = value;
    list->elems[list->head].next_idx = 0;

    return LIST_OK;
}


int list_pop(struct list *list, list_elem_t *value) {
    assert(value != NULL);
    list_error(list);

    *value = list->elems[list->head].value;
    list->elems[list->head].value = LIST_POISON;
    list->head = list->elems[list->head].prev_idx;

    return LIST_OK;
}



int list_insert(struct list *list, int position, list_elem_t value) {
    list_error(list);

    if (position < 0)                       return LIST_UNDERFLOW;
    if ((size_t) position >= list->size)    return LIST_OVERFLOW;

    list->elems[list->free_idx].value = value;
    list->elems[list->free_idx].next_idx = list->elems[position].next_idx;

    list->free_idx = list->free_idx + 1; // TODO: redo
    list->elems[position].next_idx = list->free_idx;

    return LIST_OK;
}
