// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#include "common.h"


// 排序接口
typedef struct {
    int (*mpf_sort)(void const *, int, int);
} sort_t;


// 常用排序实现
extern compare_swap_t const CMP_SWAP_OF_INT;

extern int insert_sort(void *pa_data,
                       int ele_size,
                       int count,
                       compare_swap_t const *pc_compare); // 插入排序
extern int quick_sort(void *pa_data,
                      int ele_size,
                      int count,
                      compare_swap_t const *pc_compare); // 快速排序
