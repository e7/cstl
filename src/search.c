#include "search.h"


// 顺序查找
int sequential_search(int const A_BUF[], int size, int const KEY)
{
    int index = INVALID_INDEX;

    if (NULL == A_BUF) {
        goto FINAL;
    }

    if (size < 1) {
        goto FINAL;
    }

    for (int i = 0; i < size; ++i) {
        if (KEY == A_BUF[i]) {
            index = i;
            break;
        }
    }

FINAL:
    return index;
}

// 二分查找
int binary_search(int const A_BUF[], int size, int const KEY)
{
    int index = INVALID_INDEX;
    int tmp_size = 0;
    int *p_tmp_buf = NULL;

    if (NULL == A_BUF) {
        goto FINAL;
    }

    if (size < 1) {
        goto FINAL;
    }
    
    tmp_size = size;
    p_tmp_buf = A_BUF;
    while (tmp_size > 0) {
        int i = tmp_size / 2;

        ASSERT(NULL != p_tmp_buf);

        if (KEY == p_tmp_buf[i]) {
            index = &p_tmp_buf[i] - A_BUF;
            break;
        } else if (KEY > p_tmp_buf[i]) {
            tmp_size -= &p_tmp_buf[i] - p_tmp_buf + 1;
            p_tmp_buf = &p_tmp_buf[i + 1];
        } else {
            tmp_size = &p_tmp_buf[i] - p_tmp_buf;
        }
    }

FINAL:
    return index;
}
