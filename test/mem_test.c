#include "mem.h"
#include "search.h"
#include "list.h"
#include "sort.h"


#if 0
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
    mempool_t mempool_for_test = {
        {
            {0},
        }, // ma_obj_cache[0]
    };
    ldlist_t list = {
        NULL,
    };
    int *pa_x = NULL;

    if (0 != MEMPOOL_BUILD(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }

    ldlist_build(&list, &mempool_for_test, sizeof(int));

    for (int i = 0; i < ARRAY_COUNT(buf2); ++i) {
        ldlist_push_front(&list, &buf[2]);
    }
    for (iterator_t *p_iter = ldlist_begin(&list);
         ldlist_end(&list) != p_iter;
         p_iter = (*p_iter->mpf_get_next)(p_iter))
    {
        //fprintf(stderr, "%d ", *(int *)(*p_iter->mpf_get_data(p_iter)));
    }
    fprintf(stderr, "\b\n");

    if (0 == (*sort_obj.mpf_sort)(buf2, ARRAY_COUNT(buf2))) {
        for (int i = 0; i < ARRAY_COUNT(buf2); ++i) {
            fprintf(stderr, "%d ", buf2[i]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "init success!\n");

    pa_x = MEMPOOL_ARRAY_ALLOC(&mempool_for_test, sizeof(int), 4096);
    pa_x[4095] = 23;
    pa_x[0] = 45;
    //pa_x[4096] = 50;

    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 9999));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 1112));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 7245));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 873));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 4));
    fprintf(stderr, "index: %d\n",
            (*search_obj.mpf_search)(buf, 10, 545));
    MEMPOOL_FREE(&mempool_for_test, pa_x);

    ldlist_destroy(&list);
    MEMPOOL_DESTROY(&mempool_for_test);

FINAL:
    return rslt;
}
#endif

int main(int argc, char *argv[])
{
    int rslt = 0;
    int buf[] = {
        354, 53, 422, 656, 23,
        323, 113, 5, 562, 90,
    };
    mempool_t mempool_for_test = {
        {
            {0},
        }, // ma_obj_cache[0]
    };
    ldlist_t list = {
        NULL,
    };

    if (0 != MEMPOOL_BUILD(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }

    ldlist_build(&list, &mempool_for_test, sizeof(int));

    for (int i = 0; i < ARRAY_COUNT(buf); ++i) {
        ldlist_push_front(&list, &buf[i]);
    }
    for (iterator_t *p_iter = ldlist_begin(&list);
         !(*p_iter->mpf_equal)(ldlist_end(&list), p_iter);
         p_iter = (*p_iter->mpf_get_next)(p_iter))
    {
        int *p_value = (*p_iter->mpf_get_data)(p_iter);
        fprintf(stderr, "%d ", *p_value);
    }
    fprintf(stderr, "\n");

    ldlist_destroy(&list);
    MEMPOOL_DESTROY(&mempool_for_test);

FINAL:
    return rslt;
}
