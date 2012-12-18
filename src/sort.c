// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"
#include "sort.h"
#include "list_frame.h"


// ********** 常用比较交换体 **********
// ***** 整型比较交换体 *****
// error_info: E_NULL_POINTER;
static int_t compare_int(int_t const *pc_a, int_t const *pc_b)
{
    int_t rslt = E_OK;

    ASSERT(NULL != pc_a);
    ASSERT(NULL != pc_b);

    if ((*pc_a) > (*pc_b)) {
        rslt = CMP_GREATER_THAN;
    } else if ((*pc_a) < (*pc_b)) {
        rslt = CMP_LESS_THAN;
    } else {
        rslt = CMP_EQUAL;
    }

    return rslt;
}

static void swap_int(int_t *p_a, int_t *p_b)
{
    ASSERT(NULL != p_a);
    ASSERT(NULL != p_b);

    (*p_a) ^= (*p_b);
    (*p_b) ^= (*p_a);
    (*p_a) ^= (*p_b);

    return;
}

compare_swap_t const CMP_SWAP_OF_INT = {
    (int_t (*)(void const *, void const *))&compare_int,
    (void (*)(void *, void *))&swap_int,
};


// ********** 内排序 **********
#define COUNT_CUTOFF            5

// ***** 插入排序 *****
// error_info: E_NULL_POINTER, E_OUT_OF_RANGE
int_t insert_sort(void *pa_data,
                  int_t ele_size,
                  int_t total_size,
                  compare_swap_t const *pc_compare)
{
    // 参数检查
    ASSERT(NULL != pa_data);
    ASSERT(ele_size > 0);
    ASSERT(total_size >= ele_size);
    ASSERT(NULL != pc_compare);
    ASSERT(NULL != pc_compare->mpf_compare);
    ASSERT(NULL != pc_compare->mpf_swap);

    for (int_t i = 0; i < (total_size - ele_size); i += ele_size) {
        byte_t *p_i =  &((byte_t *)pa_data)[i];

        for (int_t j = i + ele_size; j < total_size; j += ele_size) {
            int_t cmp_rslt = 0;
            byte_t *p_j =  &((byte_t *)pa_data)[j];

            cmp_rslt = (*pc_compare->mpf_compare)(p_i, p_j);
            if (CMP_GREATER_THAN == cmp_rslt) {
                (*pc_compare->mpf_swap)(p_i, p_j);
            }
        }
    }

    return 0;
}

// ***** 快速排序 *****
static inline
void *prepare_pivot(void *pa_data,
                    int_t ele_size,
                    int_t total_size,
                    compare_swap_t const *pc_compare)
{
    int_t cmp_rslt = 0;
    int_t const LEFT = 0;
    int_t const CENTER = total_size / ele_size / 2;
    int_t const RIGHT = total_size - ele_size;
    byte_t *pa_data_tmp = (byte_t *)pa_data;

    ASSERT(NULL != pa_data);
    ASSERT(ele_size > 0);
    ASSERT(total_size >= ele_size);
    ASSERT(NULL != pc_compare);
    ASSERT(NULL != pc_compare->mpf_compare);
    ASSERT(NULL != pc_compare->mpf_swap);

    cmp_rslt = (*pc_compare->mpf_compare)(&pa_data_tmp[LEFT],
                                          &pa_data_tmp[CENTER]);
    if (CMP_GREATER_THAN == cmp_rslt) {
        (*pc_compare->mpf_swap)(&pa_data_tmp[LEFT],
                                &pa_data_tmp[CENTER]);
    }
    cmp_rslt = (*pc_compare->mpf_compare)(&pa_data_tmp[LEFT],
                                          &pa_data_tmp[RIGHT]);
    if (CMP_GREATER_THAN == cmp_rslt) {
        (*pc_compare->mpf_swap)(&pa_data_tmp[LEFT],
                                &pa_data_tmp[RIGHT]);
    }
    cmp_rslt = (*pc_compare->mpf_compare)(&pa_data_tmp[CENTER],
                                          &pa_data_tmp[RIGHT]);
    if (CMP_GREATER_THAN == cmp_rslt) {
        (*pc_compare->mpf_swap)(&pa_data_tmp[CENTER],
                                &pa_data_tmp[RIGHT]);
    }
    (*pc_compare->mpf_swap)(&pa_data_tmp[CENTER], &pa_data_tmp[RIGHT]);

    return &pa_data_tmp[RIGHT];
}

// error_info: E_NULL_POINTER, E_OUT_OF_RANGE
int_t quick_sort(void *pa_data,
                 int_t ele_size,
                 int_t total_size,
                 compare_swap_t const *pc_compare)
{
    typedef struct {
        ldlist_frame_node_t m_node;
        byte_t *mpa_data;
        int_t m_total_size;
    } qs_frame_t; // 快排帧

    int_t rslt = 0;
    byte_t *p_pivot = NULL; // 枢纽元
    byte_t *p_i = NULL;
    byte_t *p_j = NULL;
    mempool_t *p_public_pool = NULL; // 公共内存池
    LDLIST_FRAME_HEAD(recursion_stack); // 递归栈
    qs_frame_t *p_in_frame = NULL;
    qs_frame_t *p_out_frame = NULL;

    // 参数检查
    ASSERT(NULL != pa_data);
    ASSERT(ele_size > 0);
    ASSERT(total_size >= ele_size);
    ASSERT(NULL != pc_compare);
    ASSERT(NULL != pc_compare->mpf_compare);
    ASSERT(NULL != pc_compare->mpf_swap);

    // start work
    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));

    // 初始化递归栈
    p_in_frame = (qs_frame_t *)MEMPOOL_ALLOC(p_public_pool,
                                             sizeof(qs_frame_t));
    if (NULL == p_in_frame) {
        rslt = -E_OUT_OF_MEM;

        goto FINAL;
    }
    init_ldlist_frame_node(&p_in_frame->m_node);
    p_in_frame->mpa_data = pa_data;
    p_in_frame->m_total_size = total_size;
    ldlist_frame_add_head(&recursion_stack, &p_in_frame->m_node);

    // 循环处理递归栈
    while (!ldlist_frame_node_alone(&recursion_stack)) {
        // 从栈中取出递归体
        p_out_frame = CONTAINER_OF(ldlist_frame_first(&recursion_stack),
                                   qs_frame_t,
                                   m_node);
        ldlist_frame_del(&p_out_frame->m_node);
        ASSERT(NULL != p_out_frame);

        // 排序
        do {
            if ((p_out_frame->m_total_size / ele_size) > COUNT_CUTOFF) {
                p_pivot = (byte_t *)prepare_pivot(p_out_frame->mpa_data,
                                                  ele_size,
                                                  p_out_frame->m_total_size,
                                                  pc_compare);
                p_i = &((byte_t *)p_out_frame->mpa_data)[0];
                p_j = &((byte_t *)p_out_frame->mpa_data)[
                          p_out_frame->m_total_size - 2 * ele_size
                      ];

                while (TRUE) {
                    int_t cmp_rslt = 0;

                    while (TRUE) {
                        cmp_rslt = (*pc_compare->mpf_compare)(p_i, p_pivot);
                        if (CMP_LESS_THAN != cmp_rslt) {
                            break;
                        }
                        p_i += ele_size;
                    }

                    while (TRUE) {
                        cmp_rslt = (*pc_compare->mpf_compare)(p_j, p_pivot);
                        if (CMP_GREATER_THAN != cmp_rslt) {
                            break;
                        }
                        p_j -= ele_size;
                    }

                    if (p_i < p_j) {
                        (*pc_compare->mpf_swap)(p_i, p_j);
                    } else {
                        break;
                    }
                }

                // 枢纽元归位
                (*pc_compare->mpf_swap)(p_i, p_pivot);

                // 压入新的递归体，现在p_i是枢纽元
                // left
                p_in_frame = (qs_frame_t *)MEMPOOL_ALLOC(p_public_pool,
                                                         sizeof(qs_frame_t));
                init_ldlist_frame_node(&p_in_frame->m_node);
                p_in_frame->mpa_data = p_out_frame->mpa_data;
                p_in_frame->m_total_size = p_i - p_out_frame->mpa_data;
                ldlist_frame_add_head(&recursion_stack, &p_in_frame->m_node);

                // right
                p_in_frame = (qs_frame_t *)MEMPOOL_ALLOC(p_public_pool,
                                                         sizeof(qs_frame_t));
                init_ldlist_frame_node(&p_in_frame->m_node);
                p_in_frame->mpa_data = p_i + ele_size;
                p_in_frame->m_total_size
                    = p_out_frame->m_total_size
                          - (p_i + ele_size - p_out_frame->mpa_data);
                ldlist_frame_add_head(&recursion_stack, &p_in_frame->m_node);

                break;
            } else {
                insert_sort(p_out_frame->mpa_data,
                            ele_size,
                            p_out_frame->m_total_size,
                            pc_compare);
            }
        } while (0);

        // 释放递归体
        ASSERT(0 == MEMPOOL_FREE(p_public_pool, p_out_frame));
    }

FINAL:
    return rslt;
}

sort_t const DEFAULT_SORT = {
    &quick_sort, // 默认使用快速排序
}; // 默认排序体
