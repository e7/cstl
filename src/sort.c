// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "sort.h"


// 冒泡排序
int bubble_sort(int a_buf[], int const SIZE)
{
    int rslt = 0;

    if (NULL == a_buf) {
        rslt = -1;
        goto FINAL;
    }

    if (SIZE < 1) {
        rslt = -1;
        goto FINAL;
    }

    for (int i = SIZE - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (a_buf[j] > a_buf[j + 1]) {
                SWAP(a_buf[j], a_buf[j + 1]);
            }
        }
    }

FINAL:
    return rslt;
}
