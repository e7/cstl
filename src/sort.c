// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "error_info.h"
#include "sort.h"


// 整型比较交换体
// error_info: E_NULL_POINTER;
static int compare_int(int const *pc_a, int const *pc_b, int *p_rslt)
{
    int rslt = E_OK;

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

static int swap_int(int *p_a, int *p_b)
{
    int rslt = E_OK;

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
    (int (*)(void const *, void const *, int *))&compare_int,
    (int (*)(void *, void *))&swap_int,
};


// 插入排序
// error_info: E_NULL_POINTER, E_OUT_OF_RANGE
int insert_sort(void *pa_data,
                int ele_size,
                int total_size,
                compare_swap_t const *pc_compare)
{
    int rslt = E_OK;
    byte_t *p_i = NULL;
    byte_t *p_j = NULL;

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

    ASSERT(NULL == p_i);
    ASSERT(NULL == p_j);
    for (int i = 0; i < (total_size - ele_size); i += ele_size) {

        p_i = &((byte_t *)pa_data)[i];
        for (int j = i + ele_size; j < total_size; j += ele_size) {
            int cmp_rslt = 0;

            p_j = &((byte_t *)pa_data)[j];
            rslt = (*pc_compare->mpf_compare)(p_i, p_j, &cmp_rslt);
            if (0 == (*pc_compare->mpf_compare)(p_i, p_j, &cmp_rslt)) {
                if (CMP_GREATER_THAN == cmp_rslt) {
                    (*pc_compare->mpf_swap)(p_i, p_j);
                }
            } else {
                goto FINAL; // like exit(-1);
            }
        }
    }

FINAL:
    return rslt;
}
