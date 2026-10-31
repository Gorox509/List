#include "list.hpp"
#include <assert.h>


int list_error(struct list *list) {
    int err = LIST_OK;

    if ((err = list_check(list)) != LIST_OK) {
        list_dump(list, err);
        return err;
    }

    return LIST_OK;
}


int list_check(struct list *list) {
    if (list == NULL)
        return LIST_NULL_PTR;
    if (list->elems == NULL)
        return LIST_DATA_NULL_PTR;
    if (list->size < 1)
        return LIST_UNDERFLOW;
    if (list->size <= TAIL(list) || TAIL(list) < 1)
        return LIST_INVALID_TAIL;
    if (list->size <= HEAD(list) || HEAD(list) < 1)
        return LIST_INVALID_HEAD;
    if (list->size <= FREE(list))
        return LIST_INVALID_FIRST_FREE;
    if (list->elems[0].value != LIST_POISON)
        return LIST_INVALID_TERMINATOR;

    return LIST_OK;
}


bool list_is_last(struct list *list, size_t idx) {
    list_error(list);

    if (list->elems[idx].next_idx == 0) return true;
    return false;
}


int list_push_back(struct list *list, list_elem_t value) { // OK
    EXIT_ERR(list_error(list));

    if (FREE(list) == 0)
        return LIST_OVERFLOW;

    size_t head = HEAD(list);

    HEAD(list) = FREE(list);
    EXIT_ERR(list_error(list));
    FREE(list) = PREV(list, HEAD(list));
    EXIT_ERR(list_error(list));

    NEXT(list, head) = HEAD(list);

    NEXT(list, HEAD(list)) = 0;
    VALUE(list, HEAD(list)) = value;
    PREV(list, HEAD(list)) = head;

    EXIT_ERR(list_error(list));

    return LIST_OK;
}


int list_pop_back(struct list *list, list_elem_t *value) {
    EXIT_ERR(list_error(list));

    if (FREE(list) == HEAD(list) || FREE(list) == TAIL(list))
        return LIST_UNDERFLOW;

    if (value != NULL)
        *value = VALUE(list, HEAD(list));

    size_t new_head = PREV(list, HEAD(list));

    VALUE(list, HEAD(list)) = LIST_POISON;
    NEXT(list, PREV(list, HEAD(list))) = 0;
    NEXT(list, HEAD(list)) = LIST_NO_NEXT;

    PREV(list, HEAD(list)) = FREE(list);
    FREE(list) = HEAD(list);

    HEAD(list) = new_head;

    EXIT_ERR(list_error(list));

    return LIST_OK;
}



int list_push(struct list *list, size_t position, list_elem_t value) {
    list_error(list);


    return LIST_OK;
}
