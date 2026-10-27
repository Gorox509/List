#include "list.hpp"
#include <assert.h>



int list_check(struct list *list) {
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
    list_check(list);

    if (list->elems[idx].next_idx == LIST_NO_NEXT) return true;
    return false;
}



int list_add(struct list *list, int position, list_elem_t value) {
    list_check(list);

    list->elems[list->free_idx].value = value;
    list->elems[list->free_idx].next_idx = list->elems[position].next_idx;

    list->free_idx = list->free_idx + 1; // TODO: redo
    list->elems[position].next_idx = list->free_idx;

    return LIST_OK;
}
