#include "list.hpp"
#include <assert.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/dir.h>

#define GRAPHVIZ_DIR "graphviz/"
#define GRAPHVIZ_FILE "graphviz.txt"

#define GRAPHVIZ_LIST_OK 0
#define GRAPHVIZ_LIST_IS_NULL_PTR 1
#define GRAPHVIZ_LIST_FAILED_TO_MAKE_FILE 2
#define GRAPHVIZ_LIST_FAILED_TO_MAKE_DIR 3
#define GRAPHVIZ_LIST_FAILED_TO_OPEN_FILE 4



static FILE *open_graphviz_file(const char *filename);
static int make_graphviz_file(const char *filename);
static int graphviz_print_params(FILE *fp);
static int graphviz_print_nodes(FILE *fp, struct list *list);


int list_dump_to_graphviz(struct list *list) {
    if (list == NULL) {
        fprintf(stderr, "List can not be accessed\n");
        return LIST_NULL_PTR;
    }

    make_graphviz_file(GRAPHVIZ_DIR GRAPHVIZ_FILE);

    FILE *fp = open_graphviz_file(GRAPHVIZ_DIR GRAPHVIZ_FILE);
    if (fp == NULL) return GRAPHVIZ_LIST_FAILED_TO_OPEN_FILE;

    fprintf(fp, "digraph G {\n");
    graphviz_print_params(fp);

    graphviz_print_nodes(fp, list);

    fprintf(fp, "}\n");

    return GRAPHVIZ_LIST_OK;
}


FILE *open_graphviz_file(const char *filename) {
    assert(filename != NULL);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, MY_RED "Error: failed to open log file\n" BASE_CLR);
        return NULL;
    }

    return fp;
}


int make_graphviz_file(const char *filename) {
    assert(filename != NULL);
    int err = GRAPHVIZ_LIST_OK;

    if (system("mkdir " GRAPHVIZ_DIR)) {
        fprintf(stderr, "Warning: directory already exists or cannot be created\n");
        err = GRAPHVIZ_LIST_FAILED_TO_MAKE_DIR;
    }

    return err;
}


int graphviz_print_params(FILE *fp) {
    assert(fp != NULL);

    fprintf(fp, "rankdir=TB;\n");
    fprintf(fp, "node[fontsize=12];\n");
    fprintf(fp, "splines=ortho;\n");

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print_nodes(FILE *fp, struct list *list) {
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    fprintf(fp, "node%zu[ color = red , shape = Mrecord , label = \"{ <idx> index = 0 | value = %zd | { <next> next = %zu | <prev> prev = %zu } }\" ];\n",
            0LU, VALUE(list, 0), TAIL(list), HEAD(list));

    for (size_t idx = 1; idx < SIZE(list); ++idx) {
        fprintf(fp, "node%zu[ color = red , shape = Mrecord , label = \"{ <idx> index = %zu | value = %zd | { <next> next = %zu | <prev> prev = %zu } }\" ];\n",
                idx, idx, VALUE(list, idx), NEXT(list, idx), PREV(list, idx));

        if (!list_is_free_element(list, idx)) {// TODO: not reliable
            fprintf(fp, "node%zu->node%zu[color=\"white\", weight=100000];\n", idx - 1, idx);
        }
    }


    fprintf(fp, "node0:<next>");
    for (size_t idx = 1; idx < SIZE(list); ++idx) {
        if (!list_is_free_element(list, idx)) {
            fprintf(fp, "->node%zu:<idx>[color=\"black\"];\n", idx);
            fprintf(fp, "node%zu:<next>", idx);
        }
    }
    fputc(';', fp);
    fputc('\n', fp);

    if (FREE(list) != 0) {
        fprintf(fp, "node%zu:<prev>", FREE(list));
        for (size_t cnt = 0, idx = FREE(list); idx != 0 && cnt < SIZE(list); ++cnt, idx = PREV(list, idx)) {
            fprintf(fp, "->node%zu:<idx>[color=\"white\", weight=100000];\n", idx);
            fprintf(fp, "node%zu:<prev>", idx);
        }
    }
    fputc('\n', fp);

    return GRAPHVIZ_LIST_OK;
}
