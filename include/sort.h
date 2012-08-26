// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "common.h"


// 排序接口
typedef struct {
    int (*mf_sort)(int [], int const);
};


// 常用排序实现
int bubble_sort(int a_buf[], int const SIZE); // 冒泡排序
