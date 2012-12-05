#include "mem.h"
#include "search.h"
#include "list.h"
#include "sort.h"
#include "rbtree_frame.h"
#include "avltree_frame.h"
#include "adv_string.h"
#include "error_info.h"


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

#if 0
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
    ldlist_iterator_t list_iter = {
        NULL,
    };

    MEMPOOL_BUILD(&mempool_for_test);
    ldlist_build(&list, &mempool_for_test, sizeof(int));

    for (int i = 0; i < ARRAY_COUNT(buf); ++i) {
        ldlist_push_back(&list, &buf[i]);
    }
    /*printf("some delete =====\n");
    printf("pop exit code: %d\n", ldlist_pop_back(&list));
    printf("pop exit code: %d\n", ldlist_pop_back(&list));
    printf("pop exit code: %d\n", ldlist_pop_back(&list));
    printf("some delete =====\n");*/
    for (iterator_t *p_iter = ldlist_begin(&list, &list_iter);
         !(*p_iter->mpf_equal)(ldlist_end(&list), p_iter);
         p_iter = (*p_iter->mpf_get_next)(p_iter))
    {
        int *p_value = (*p_iter->mpf_get_data)(p_iter);
        fprintf(stderr, "%d ", *p_value);
    }
    fprintf(stderr, "\n");

    ldlist_destroy(&list);
    MEMPOOL_DESTROY(&mempool_for_test);

    return rslt;
}
#endif

#if 0
int main(int argc, char *argv[])
{
    rbtree_frame_t tree;
    rbtree_frame_node_t nodes[] = {
        {12, RB_BLACK, NULL, NULL, NULL},
        {1, RB_BLACK, NULL, NULL, NULL},
        {9, RB_BLACK, NULL, NULL, NULL},
        {2, RB_BLACK, NULL, NULL, NULL},
        {0, RB_BLACK, NULL, NULL, NULL},
       /* {11, RB_BLACK, NULL, NULL, NULL},
        {7, RB_BLACK, NULL, NULL, NULL},
        {19, RB_BLACK, NULL, NULL, NULL},
        {4, RB_BLACK, NULL, NULL, NULL},
        {15, RB_BLACK, NULL, NULL, NULL},
        {18, RB_BLACK, NULL, NULL, NULL},
        {5, RB_BLACK, NULL, NULL, NULL},
        {14, RB_BLACK, NULL, NULL, NULL},
        {13, RB_BLACK, NULL, NULL, NULL},
        {10, RB_BLACK, NULL, NULL, NULL},
        {16, RB_BLACK, NULL, NULL, NULL},
        {6, RB_BLACK, NULL, NULL, NULL},
        {3, RB_BLACK, NULL, NULL, NULL},
        {8, RB_BLACK, NULL, NULL, NULL},
        {17, RB_BLACK, NULL, NULL, NULL},*/
    };
    //rbtree_frame_node_t *p_pos = NULL;
    ldlist_t list = {
        NULL,
    };
    mempool_t mempool_for_test = {
        {
            {0},
        }, // ma_obj_cache[0]
    };

    MEMPOOL_BUILD(&mempool_for_test);
    ldlist_build(&list, &mempool_for_test, sizeof(rbtree_frame_node_t *));

    init_rbtree_frame(&tree);

    fprintf(stderr, "before insert size: %d\n", tree.m_size);
    for (int i = 0; i < ARRAY_COUNT(nodes); ++i) {
        insert_rbtree_frame(&tree, &nodes[i]);
    }
    fprintf(stderr, "after insert size: %d\n", tree.m_size);
    for (int i = 0; i < ARRAY_COUNT(nodes) - 1; ++i) {
        remove_from_rbtree_frame(&tree, nodes[i].m_key);
    }
    fprintf(stderr, "after remove size: %d\n", tree.m_size);

    // 中序遍历
    /*
    p_pos = tree.mp_root;
    while (NULL != p_pos) {
        if (NULL != p_pos->mp_lchild) {
            ldlist_push_front(&list, &p_pos);
            p_pos = p_pos->mp_lchild;
            continue;
        }

        fprintf(stderr, "%d\n", p_pos->m_key);

        if (NULL == p_pos->mp_rchild) {
            if (ldlist_is_empty(&list)) {
                break;
            }
            p_pos = *(rbtree_frame_node_t **)ldlist_first(&list);
            ldlist_pop_front(&list);
        }
        fprintf(stderr, "%d\n", p_pos->m_key);

        p_pos = p_pos->mp_rchild;
    }*/

    ldlist_destroy(&list);
    MEMPOOL_DESTROY(&mempool_for_test);

    return 0;
}
#endif

#if 0
// 2-3树性能测试
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef int key_t;

int main(int argc, char *argv[])
{
    int i, count = 10000000;
    struct timeval tpstart, tpend;
    float timeuse;
    rbtree_frame_t tree;
    rbtree_frame_node_t *p_nodes = malloc(sizeof(rbtree_frame_node_t) * count);
    //rbtree_frame_node_t *p_nodes = NULL;
    mempool_t mempool_for_test;

    MEMPOOL_BUILD(&mempool_for_test);

    //p_nodes = MEMPOOL_ARRAY_ALLOC(&mempool_for_test, count, sizeof(rbtree_frame_node_t));
    if (NULL == p_nodes) {
        return -1;
    }

    srand(time(NULL));
    init_rbtree_frame(&tree);

    for (i = 0; i < count; ++i) {
        p_nodes[i].m_key = rand() % count;
    }

    printf("start\n");
    gettimeofday(&tpstart, NULL);
    for (i = 0; i < count; ++i) {
        insert_rbtree_frame(&tree, &p_nodes[i]);
    }
    gettimeofday(&tpend, NULL);
    printf("stop\n");

    timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec)
                  + (tpend.tv_usec - tpstart.tv_usec);
    timeuse /= 1000000;

    printf("Used Time: %f\n", timeuse);

    MEMPOOL_DESTROY(&mempool_for_test);

    return 0;
}
#endif

#if 1
// avl树性能测试
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef int key_t;

int main(int argc, char *argv[])
{
    //int count = 10000000;
    int count = 50;
    struct timeval tpstart, tpend;
    float timeuse;
    avltree_frame_t *p_tree = NULL;
    avltree_frame_t *p_nodes = NULL;
    int *p_keys = (int *)calloc(count, sizeof(int));
    int keys[] = {
        2, 2, 41, 8, 15, 3, 33, 47, 8, 28, 42,
        17, 47, 30, 26, 7, 24, 48, 22, 6, 6,
        42, 19, 42, 23, 25, 5, 28, 25, 12, 34,
        27, 16, 27, 35, 31, 32, 19, 28, 40, 49,
        20, 9, 46, 2, 36, 5, 28, 36, 28,
    };
    avltree_frame_t nodes[] = {
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 10
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 15
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 20
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 30
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 35
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 40
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 45
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL},
        {0, NULL, 0, NULL, NULL, NULL}, // 50
    };
    mempool_t mempool_for_test;

    MEMPOOL_BUILD(&mempool_for_test);
    srand(time(NULL));

    //p_nodes = MEMPOOL_ARRAY_ALLOC(&mempool_for_test, count, sizeof(rbtree_frame_node_t));
    p_nodes = calloc(count, sizeof(avltree_frame_t));

    for (int i = 0; i < count; ++i) {
        p_keys[i] =  rand() % count;

        p_nodes[i].m_key = p_keys[i];
        p_nodes[i].mp_ftree = NULL;
        p_nodes[i].mp_ltree = NULL;
        p_nodes[i].mp_rtree = NULL;
    }

    /*fprintf(stderr, "keys: ");
    for (int i = 0; i < count; ++i) {
        fprintf(stderr, "%d, ", p_keys[i]);
    }*/
    fprintf(stderr, "\b\n");

    printf("start\n");
    gettimeofday(&tpstart, NULL);
    /*for (int i = 0; i < count; ++i) {
        fprintf(stderr, "insert[%d]: %d\n", i, p_nodes[i].m_key);
        insert_avltree_frame(&p_tree, &p_nodes[i]);
    }

    for (int i = 0; i < count; ++i) {
        avl_iter_t iter = {NULL, NULL};
        fprintf(stderr, "find[%d]: %d\n", i, p_nodes[i].m_key);
        find_avltree_frame(&p_tree, p_keys[i], &iter);
        if (NULL != (*iter.mpp_child)) {
            if (ABS((*iter.mpp_child)->m_balance_factor) > 1) {
                ASSERT(0);
            }
        }
    }

    for (int i = 0; i < count; ++i) {
        avl_iter_t iter = {NULL, NULL};

        fprintf(stderr, "%d: %d", i, p_keys[i]);
        find_avltree_frame(&p_tree, p_keys[i], &iter);
        if (NULL != (*iter.mpp_child)) {
            fprintf(stderr, " [YES]\n");
            if (ABS((*iter.mpp_child)->m_balance_factor) > 1) {
                ASSERT(0);
            }
            remove_avltree_frame(&p_tree, p_keys[i]);
            if (NULL == p_tree) {
                break;
            }
        } else {
            fprintf(stderr, " [NO]\n");
        }
    }*/

    for (int i = 0; i < ARRAY_COUNT(keys); ++i) {
        nodes[i].m_key = keys[i];
    }
    for (int i = 0; i < ARRAY_COUNT(nodes); ++i) {
        insert_avltree_frame(&p_tree, &nodes[i]);
    }
    for (int i = 0; i < ARRAY_COUNT(nodes); ++i) {
        fprintf(stderr, "remove[%d]: %d\n", i, keys[i]);
        remove_avltree_frame(&p_tree, keys[i]);
        if (NULL == p_tree) {
            break;
        }
    }
    gettimeofday(&tpend, NULL);
    printf("stop\n");

    timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec)
                  + (tpend.tv_usec - tpstart.tv_usec);
    timeuse /= 1000000;

    printf("Used Time: %f\n", timeuse);

    MEMPOOL_DESTROY(&mempool_for_test);

    return 0;
}
#endif
