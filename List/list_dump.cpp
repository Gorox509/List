#include "list.hpp"
#include <assert.h>
#include <stdio.h>


static FILE *open_log_file( const char *filename);
static int  close_log_file( FILE *fp);
static void print_divisor(  FILE *fp);


void list_dump(struct list *list) {
    size_t idx = 0;

    FILE *fp = open_log_file("list_log.log");
    if (fp == NULL) return;

    print_divisor(fp);

    if (list == NULL) {
        fprintf(fp, "List can not be accessed\n");
        return;
    }

    fprintf(fp, "Dumping list structure:\n");

    fprintf(fp, "Size: %zu\n", list->size);
    fprintf(fp, "Head: %d\n",  list->head);
    fprintf(fp, "Tail: %d\n",  list->tail);
    fprintf(fp, "Free: %d\n",  list->free_idx);

    fprintf(fp, "\nList data at [%p]:\n", list->elems);

    if (list->elems == NULL) {
        fprintf(fp, "Can not print list elements\n");
    }

    fprintf(fp, "%10s%10s%10s%10s\n", "idx", "data", "next_idx", "prev_idx");

    while (idx < list->size && !list_is_last(list, (short) idx)) {
        fprintf(fp, "%10zu%10d%10d%10d\n", idx, list->elems[idx].value, list->elems[idx].next_idx, list->elems[idx].prev_idx);
        ++idx;
    }

    print_divisor(fp);

    close_log_file(fp);

    return;
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
