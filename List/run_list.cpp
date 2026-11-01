#include "list.hpp"
#include <assert.h>
#include <stdio.h>


static int run_list(int argc, char *argv[]);



int main(int argc, char *argv[]) {
    return run_list(argc, argv);
}


int run_list(int argc, char *argv[]) {
    assert(argv != NULL);
    assert(argc > 1);

    struct list *list = NULL;
    make_list(&list, strtoul(argv[1], NULL, 10));

    list_push_back(list, 3);
    list_push_back(list, 4);
    list_push_back(list, 6);
    list_push_back(list, 8);
    //list_push_back(list, 9);
    list_push(list, 2, 9);

    ssize_t value = 0;
    //list_pop_back(list, &value);
    fprintf(stderr, "%zd\n", value);

    value = 0;
    //list_pop_back(list, &value);
    fprintf(stderr, "%zd\n", value);

    list_dump(list, LIST_OK);

    list_free(list);

    return 0;
}
