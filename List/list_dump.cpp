#include "list.hpp"
#include <assert.h>
#include <stdio.h>


static FILE *open_log_file( const char *filename);
static int  close_log_file( FILE *fp);
static void print_divisor(  FILE *fp);
static void list_print_error(FILE *fp, int err_code);


void list_dump(struct list *list, int error) { // TODO: get error
    size_t idx = 0;

    FILE *fp = open_log_file("list_log.log");
    if (fp == NULL) return;

    print_divisor(fp);

    list_print_error(fp, error);

    if (list == NULL) {
        fprintf(fp, "List can not be accessed\n");
        return;
    }

    fprintf(fp, "Dumping list structure:\n");

    fprintf(fp, "Size: %zu\n", list->size);
    fprintf(fp, "Free: %zu\n", FREE(list));

    fprintf(fp, "\nList data at [%p]:\n", list->elems);

    if (list->elems == NULL) {
        fprintf(fp, "Can not print list elements\n");
    }

    fprintf(fp, "%10s%10s%10s%10s\n", "idx", "data", "next_idx", "prev_idx");

    while (idx < list->size) {
        fprintf(fp, "%10zu%10zd%10zu%10zu\n", idx, VALUE(list, idx), NEXT(list, idx), PREV(list, idx));
        ++idx;
    }

    print_divisor(fp);

    close_log_file(fp);

    return;
}


void list_print_error(FILE *fp, int err_code) {
    assert(fp != NULL);

    switch (err_code) {
        case LIST_OK:
            return;

        case LIST_ALLOC_FAILED:
            fprintf(fp, "Error: failed to allocate list\n");
            return;

        case LIST_DATA_ALLOC_FAILED:
            fprintf(fp, "Error: failed to allocate data array\n");
            return;

        case LIST_NULL_PTR:
            fprintf(fp, "Error: list at NULL address\n");
            return;

        case LIST_DATA_NULL_PTR:
            fprintf(fp, "Error: list elements at NULL\n");
            return;

        case LIST_INVALID_HEAD:
            fprintf(fp, "Error: head is invalid\n");
            return;

        case LIST_INVALID_TAIL:
            fprintf(fp, "Error: tail is invalid\n");
            return;

        case LIST_INVALID_FIRST_FREE:
            fprintf(fp, "Error: first free element index is invalid\n");
            return;

        case LIST_UNDERFLOW:
            fprintf(fp, "Error: list underflow\n");
            return;

        case LIST_OVERFLOW:
            fprintf(fp, "Error: list overflow\n");
            return;

        case LIST_INVALID_TERMINATOR:
            fprintf(fp, "Error: list terminator element is wrong\n");
            return;

        default:
            fprintf(fp, "Unknown error\n");
            return;
    }
}


FILE *open_log_file(const char *filename) {
    assert(filename != NULL);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, MY_RED "Error: failed to open log file\n" BASE_CLR);
        return NULL;
    }

    return fp;
}


int close_log_file(FILE *fp) {
    assert(fp != NULL);

    return fclose(fp);
}


void print_divisor(FILE *fp) {
    assert(fp != NULL);

    fprintf(fp, "======================================================================"
                "==============================\n");
}
