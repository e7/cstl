// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"
#include "sort.h"
#include "list_frame.h"


// ********** 常用比较交换体 **********
// ***** 整型比较交换体 *****
// error_info: E_NULL_POINTER;
static int_t compare_int(int_t const *pc_a, int_t const *pc_b, int_t *p_rslt)
{
    int_t rslt = E_OK;

    if ((NULL == pc_a) || (NULL == pc_b)) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ((*pc_a) > (*pc_b)) {
        *p_rslt = CMP_GREATER_THAN;
    } else if ((*pc_a) < (*pc_b)) {
        *p_rslt = CMP_LESS_THAN;
    } else {
        *p_rslt = CMP_EQUAL;
    }

FINAL:
    return rslt;
}

static int_t swap_int(int_t *p_a, int_t *p_b)
{
    int_t rslt = E_OK;

    if ((NULL == p_a) || (NULL == p_b)) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }
    (*p_a) ^= (*p_b);
    (*p_b) ^= (*p_a);
    (*p_a) ^= (*p_b);

FINAL:
    return rslt;
}

compare_swap_t const CMP_SWAP_OF_INT = {
    (int_t (*)(void const *, void const *, int_t *))&compare_int,
    (int_t (*)(void *, void *))&swap_int,
};


// ********** 内排序 **********
#define COUNT_CUTOFF            20

int_t default_sort(void *pa_data,
                   ssize_t ele_size,
                   ssize_t total_size,
                   compare_swap_t const *pc_compare)
{
    int_t rslt = E_OK;

    if (NULL == pa_data) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ((ele_size < 0) || (total_size < ele_size)) {
        rslt = -E_OUT_OF_RANGE;

        goto FINAL;
    }

    if (NULL == pc_compare) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ( (NULL == pc_compare->mpf_compare) ||
            (NULL == pc_compare->mpf_swap))
    {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ((total_size / ele_size) < COUNT_CUTOFF) {
        rslt = insert_sort(pa_data, ele_size, total_size, pc_compare);
    } else {
        rslt = quick_sort(pa_data, ele_size, total_size, pc_compare);
    }

FINAL:
    return rslt;
}


// ***** 插入排序 *****
// error_info: E_NULL_POINTER, E_OUT_OF_RANGE
int_t insert_sort(void *pa_data,
                  ssize_t ele_size,
                  ssize_t total_size,
                  compare_swap_t const *pc_compare)
{
    int_t rslt = E_OK;

    if (NULL == pa_data) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ((ele_size < 0) || (total_size < ele_size)) {
        rslt = -E_OUT_OF_RANGE;

        goto FINAL;
    }

    if (NULL == pc_compare) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ( (NULL == pc_compare->mpf_compare) ||
            (NULL == pc_compare->mpf_swap))
    {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    for (int_t i = 0; i < (total_size - ele_size); i += ele_size) {
        byte_t *p_i =  &((byte_t *)pa_data)[i];

        for (int_t j = i + ele_size; j < total_size; j += ele_size) {
            int_t cmp_rslt = 0;
            byte_t *p_j =  &((byte_t *)pa_data)[j];

            rslt = (*pc_compare->mpf_compare)(p_i, p_j, &cmp_rslt);
            ASSERT(0 == (*pc_compare->mpf_compare)(p_i, p_j, &cmp_rslt));
            if (CMP_GREATER_THAN == cmp_rslt) {
                (*pc_compare->mpf_swap)(p_i, p_j);
            }
        }
    }

FINAL:
    return rslt;
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
    ASSERT(total_size > ele_size);
    ASSERT(NULL != pc_compare);
    ASSERT(NULL != pc_compare->mpf_compare);
    ASSERT(NULL != pc_compare->mpf_swap);

    ASSERT(0 == (*pc_compare->mpf_compare)(&pa_data_tmp[LEFT],
                                           &pa_data_tmp[CENTER],
                                           &cmp_rslt));
    if (CMP_GREATER_THAN == cmp_rslt) {
        ASSERT(0 == (*pc_compare->mpf_swap)(&pa_data_tmp[LEFT],
                                            &pa_data_tmp[CENTER]));
    }
    ASSERT(0 == (*pc_compare->mpf_compare)(&pa_data_tmp[LEFT],
                                           &pa_data_tmp[RIGHT],
                                           &cmp_rslt));
    if (CMP_GREATER_THAN == cmp_rslt) {
        ASSERT(0 == (*pc_compare->mpf_swap)(&pa_data_tmp[LEFT],
                                            &pa_data_tmp[RIGHT]));
    }
    ASSERT(0 == (*pc_compare->mpf_compare)(&pa_data_tmp[CENTER],
                                           &pa_data_tmp[RIGHT],
                                           &cmp_rslt));
    if (CMP_GREATER_THAN == cmp_rslt) {
        ASSERT(0 == (*pc_compare->mpf_swap)(&pa_data_tmp[CENTER],
                                            &pa_data_tmp[RIGHT]));
    }
    ASSERT(0 == (*pc_compare->mpf_swap)(&pa_data_tmp[CENTER],
                                        &pa_data_tmp[RIGHT]));

    return &pa_data_tmp[RIGHT];
}

// error_info: E_NULL_POINTER, E_OUT_OF_RANGE
int_t quick_sort(void *pa_data,
                 ssize_t ele_size,
                 ssize_t total_size,
                 compare_swap_t const *pc_compare)
{
    typedef struct {
        ldlist_frame_node_t m_node;
        void *mpa_data;
        ssize_t total_size;
    } qs_frame_t; // 快排帧

    int_t rslt = 0;
    byte_t *p_pivot = NULL; // 枢纽元
    byte_t *p_i = NULL;
    byte_t *p_j = NULL;
    mempool_t *p_public_pool = NULL; // 公共内存池

    if (NULL == pa_data) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    // 元素数目小于3时不能做快速排序
    if ((ele_size < 0) ||
            (total_size < ele_size) ||
            ((total_size / ele_size) < 3))
    {
        rslt = -E_OUT_OF_RANGE;

        goto FINAL;
    }

    if (NULL == pc_compare) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if ((NULL == pc_compare->mpf_compare) ||
            (NULL == pc_compare->mpf_swap))
    {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    // start work
    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    while (TRUE) {
        p_pivot = (byte_t *)prepare_pivot(pa_data,
                                          ele_size,
                                          total_size,
                                          pc_compare);
        p_i = &((byte_t *)pa_data)[0];
        p_j = &((byte_t *)pa_data)[total_size - 2 * ele_size];

        fprintf(stderr, "[i] %p, [j] %p, [pivot] %p", p_i, p_j, p_pivot);
    }

FINAL:
    return rslt;
}

sort_t const DEFAULT_SORT = {
    &default_sort,
};
