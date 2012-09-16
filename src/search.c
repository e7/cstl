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
int binary_search(int const A_BUF[], int const SIZE, int const KEY)
{
    int index = INVALID_INDEX;
    int tmp_size = 0;
    int const *pc_tmp_buf = NULL;

    if (NULL == A_BUF) {
        goto FINAL;
    }

    if (SIZE < 1) {
        goto FINAL;
    }

    tmp_size = SIZE;
    pc_tmp_buf = A_BUF;
    while (tmp_size > 0) {
        int i = tmp_size / 2;

        ASSERT(NULL != pc_tmp_buf);

        if (KEY == pc_tmp_buf[i]) {
            index = &pc_tmp_buf[i] - A_BUF;
            break;
        } else if (KEY > pc_tmp_buf[i]) {
            tmp_size -= &pc_tmp_buf[i] - pc_tmp_buf + 1;
            pc_tmp_buf = &pc_tmp_buf[i + 1];
        } else {
            tmp_size = &pc_tmp_buf[i] - pc_tmp_buf;
        }
    }

FINAL:
    return index;
}
