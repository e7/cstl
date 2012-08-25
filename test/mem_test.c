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

    if (0 != mempool_build(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }
    fprintf(stderr, "init success!\n");
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
    mempool_destroy(&mempool_for_test);

FINAL:
    return rslt;
}
