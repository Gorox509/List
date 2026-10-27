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

    list_add(list, 0, 1);
    list_add(list, 1, 5);
    list_add(list, 2, 8);
    list_add(list, 3, 13);
    list_add(list, 4, 91);

    list_dump(list);

    list_free(list);

    return 0;
}
