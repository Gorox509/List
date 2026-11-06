#include "list.hpp"
#include <assert.h>
#include <cstdio>


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
    if (DATA(list) == NULL)
        return LIST_DATA_NULL_PTR;
    if (SIZE(list) < 1)
        return LIST_UNDERFLOW;
    if (SIZE(list) <= TAIL(list))
        return LIST_INVALID_TAIL;
    if (SIZE(list) <= HEAD(list))
        return LIST_INVALID_HEAD;
    if (SIZE(list) <= FREE(list))
        return LIST_INVALID_FIRST_FREE;
    if (VALUE(list, 0) != LIST_POISON)
        return LIST_INVALID_TERMINATOR;

    return LIST_OK;
}


bool list_is_last(struct list *list, size_t idx) {
    list_error(list);

    if (NEXT(list, idx) == 0) return true;
    return false;
}


bool list_is_free_element(struct list *list, size_t idx) {
    EXIT_ERR(list_error(list));

    if (VALUE(list, idx) == LIST_POISON && NEXT(list, idx) == LIST_NO_NEXT)
        return true;

    return false;
}


int list_push_back(struct list *list, list_elem_t value) {
    EXIT_ERR(list_error(list));

    return list_push(list, HEAD(list), value);
}


int list_pop_back(struct list *list, list_elem_t *value) {
    EXIT_ERR(list_error(list));

    return list_pop(list, HEAD(list), value);
}



int list_push(struct list *list, size_t position, list_elem_t value) {
    EXIT_ERR(list_error(list));

    if (position >= SIZE(list) - 2)
        return LIST_CANT_PUSH;
    if (FREE(list) == 0)
        return LIST_OVERFLOW;

    size_t idx_to_push_into = FREE(list);

    idx_to_push_into = FREE(list);
    EXIT_ERR(list_error(list));
    FREE(list) = PREV(list, FREE(list));
    EXIT_ERR(list_error(list));

    PREV(list, NEXT(list, position)) = idx_to_push_into;

    NEXT(list, idx_to_push_into) = NEXT(list, position);
    NEXT(list, position) = idx_to_push_into;
    VALUE(list, idx_to_push_into) = value;
    PREV(list, idx_to_push_into) = position;

    EXIT_ERR(list_error(list));

    return LIST_OK;
}


int list_pop(struct list *list, size_t position, list_elem_t *value) {
    EXIT_ERR(list_error(list));

    if (position < 1 || position >= SIZE(list) - 1)
        return LIST_CANT_POP;

    if (value != NULL)
        *value = VALUE(list, position);


    VALUE(list, position) = LIST_POISON;
    NEXT(list, PREV(list, position)) = NEXT(list, position);
    PREV(list, NEXT(list, position)) = PREV(list, position);
    NEXT(list, position) = LIST_NO_NEXT;

    PREV(list, position) = FREE(list);
    FREE(list) = position;

    EXIT_ERR(list_error(list));

    return LIST_OK;
}
