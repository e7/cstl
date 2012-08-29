#include "mem.h"
#include "search.h"
#include "list_frame.h"
#include "sort.h"


int main(int argc, char *argv[])
{
    int rslt = 0;
    int buf[] = {
        4, 31, 92, 123, 545,
        873, 983, 1112, 7245, 9999,
    };
    int buf2[] = {
        354, 53, 422, 656, 23,
        323, 113, 5, 562, 90,
    };
    search_t search_obj = {
        &binary_search,
    };
    sort_t sort_obj = {
        &bubble_sort,
    };
    mempool_t mempool_for_test;


    if (0 != mempool_build(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }
    if (0 == (*sort_obj.mf_sort)(buf2, ARRAY_COUNT(buf2))) {
        for (int i = 0; i < ARRAY_COUNT(buf2); ++i) {
            fprintf(stderr, "%d ", buf2[i]);
        }
        fprintf(stderr, "\n");
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
