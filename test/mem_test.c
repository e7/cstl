#include "mem.h"
#include "search.h"
#include "list_frame.h"


int main(int argc, char *argv[])
{
    int rslt = 0;
    int buf[] = {
        4, 31, 92, 123, 545,
        873, 983, 1112, 7245, 9999,
    };
    search_t search_obj = {
        &binary_search,
    };
    mempool_t mempool_for_test;
    int *pa_x = NULL;

    if (0 != mempool_build(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }
    fprintf(stderr, "init success!\n");

    pa_x = mempool_array_alloc(&mempool_for_test, sizeof(int), 4096);
    pa_x[4095] = 23;
    pa_x[0] = 45;
    //pa_x[4096] = 50;

    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 9999));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 1112));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 7245));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 873));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 4));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mf_search)(buf, 10, 545));
    mempool_free(&mempool_for_test, pa_x);
    mempool_destroy(&mempool_for_test);

FINAL:
    return rslt;
}
