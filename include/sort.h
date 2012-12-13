// sort.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __SORT_H__
#define __SORT_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


// 排序接口
typedef struct {
    int_t (*mpf_sort)(void *, ssize_t, ssize_t, compare_swap_t const *);
} sort_t;

extern sort_t const DEFAULT_SORT; // 默认排序体


// 常用排序实现
extern compare_swap_t const CMP_SWAP_OF_INT;

extern int_t insert_sort(void *pa_data,
                         ssize_t ele_size,
                         ssize_t total_size,
                         compare_swap_t const *pc_compare); // 插入排序
extern int_t quick_sort(void *pa_data,
                        ssize_t ele_size,
                        ssize_t total_size,
                        compare_swap_t const *pc_compare); // 快速排序


#ifdef __cplusplus
} // extern "C"
#endif
#endif
