#include "list.hpp"
#include <assert.h>
#include <cstdlib>


static int run_list(int argc, char *argv[]);



int main(int argc, char *argv[]) {
    return run_list(argc, argv);
}


int run_list(int argc, char *argv[]) {
    assert(argv != NULL);

    struct list *list = NULL;
    make_list(&list, strtoul(argv[1], NULL, 10));

    list_push(list, 3);
    list_push(list, 4);
    list_push(list, 6);
    list_push(list, 8);
    list_push(list, 9);

    list_dump(list);

    list_free(list);

    return 0;
}
