#include "list.hpp"
#include <assert.h>
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
static int make_graphviz_file(  const char *filename);
static int graphviz_print_params(FILE *fp);

static int graphviz_print(           FILE *fp, struct list *list);
static int graphviz_print_nodes(     FILE *fp, struct list *list);
static int graphviz_print_next_edges(FILE *fp, struct list *list);
static int graphviz_print_prev_edges(FILE *fp, struct list *list);
static int graphviz_print_free_edges(FILE *fp, struct list *list);


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

    graphviz_print(fp, list);

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
    fprintf(fp, "splines=ortho;\n"); // TODO: to ortho

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print(FILE *fp, struct list *list) {
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    graphviz_print_nodes(fp, list);

    graphviz_print_next_edges(fp, list);

    graphviz_print_prev_edges(fp, list);

    graphviz_print_free_edges(fp, list);

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print_nodes(FILE *fp, struct list *list) { // TODO: fix white edges
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    fprintf(fp, "node%zu[ color = black , style = filled , fillcolor = whitesmoke, shape = Mrecord , label = \"{ <idx> index = 0 | value = %zd | { <next> next = %zu | <prev> prev = %zu } }\" ];\n",
            0LU, VALUE(list, 0), TAIL(list), HEAD(list));

    size_t prev_not_free = 0;
    for (size_t idx = 1; idx < SIZE(list); ++idx) {
        const char *color = NULL, *fillcolor = NULL;
        bool elem_is_free = list_is_free_element(list, idx);
        if (!elem_is_free) {
            color = "darkgreen";
            fillcolor = "lightgreen";
        } else {
            color = "red";
            fillcolor = "whitesmoke";
        }

        fprintf(fp, "node%zu[ color = %s , style = filled, fillcolor = %s, shape = Mrecord , label = \"{ <idx> index = %zu | value = %zd | { <next> next = %zu | <prev> prev = %zu } }\" ];\n",
                idx, color, fillcolor, idx, VALUE(list, idx), NEXT(list, idx), PREV(list, idx));

        if (!list_is_free_element(list, idx)) {
            fprintf(fp, "node%zu->node%zu[color=\"white\", weight=100000];\n", prev_not_free, idx);
            prev_not_free = idx;
        }
    }

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print_next_edges(FILE *fp, struct list *list) {
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    fprintf(fp, "node0");
    if (PREV(list, NEXT(list, 0)) == 0) fprintf(fp, ":<idx>");
    else fprintf(fp, ":<next>");
    for (size_t idx = TAIL(list), cnt = 0; idx != 0 && cnt < SIZE(list); ++cnt, idx = NEXT(list, idx)) {
        if (!list_is_free_element(list, idx)) {
            if (PREV(list, NEXT(list, idx)) == idx) {
                fprintf(fp, "->node%zu:<idx>[color=\"green\", dir=both];\n", idx);
                fprintf(fp, "node%zu:<idx>", idx);
            }
            else {
                fprintf(fp, "->node%zu:<idx>[color=\"blue\"];\n", idx);
                fprintf(fp, "node%zu:<next>", idx);
            }
        }
    }
    fputc(';', fp);
    fputc('\n', fp);

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print_prev_edges(FILE *fp, struct list *list) {
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    fprintf(fp, "node0");
    if (PREV(list, NEXT(list, 0)) == 0) fprintf(fp, ":<idx>");
    else fprintf(fp, ":<prev>");
    for (size_t idx = HEAD(list), cnt = 0; idx != 0 && cnt < SIZE(list); ++cnt, idx = PREV(list, idx)) {
        if (!list_is_free_element(list, idx)) {
            if (PREV(list, NEXT(list, idx)) != idx) {
                fprintf(fp, "->node%zu:<idx>[color=\"magenta\"];\n", idx);
                fprintf(fp, "node%zu:<prev>", idx);
            }
        }
    }
    fputc(';', fp);
    fputc('\n', fp);

    return GRAPHVIZ_LIST_OK;
}


int graphviz_print_free_edges(FILE *fp, struct list *list) {
    assert(fp != NULL);
    EXIT_ERR(list_error(list));

    if (FREE(list) != 0) {
        fprintf(fp, "node%zu", FREE(list));
        for (size_t cnt = 0, idx = PREV(list, FREE(list)); idx != 0 && cnt < SIZE(list); ++cnt, idx = PREV(list, idx)) {
            fprintf(fp, "->node%zu[color=\"white\", weight=100000];\n", idx);
            fprintf(fp, "node%zu", idx);
        }
    }
    fputc('\n', fp);

    return GRAPHVIZ_LIST_OK;
}
