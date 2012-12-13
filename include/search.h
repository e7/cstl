// search.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __SEARCH_H__
#define __SEARCH_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


// 查找接口
#define INVALID_INDEX           (-1)

typedef struct {
    int_t (*mpf_search)(int_t const [], int_t const, int_t const);
} search_t;


// 常用查找实现
extern int_t sequential_search(int_t const A_BUF[],
                               int_t const size,
                               int_t const KEY); // 顺序查找
extern int_t binary_search(int_t const A_BUF[],
                           int_t const size,
                           int_t const KEY); // 二分查找


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __SEARCH_H__
