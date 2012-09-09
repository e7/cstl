// search.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __SEARCH_H__
#define __SEARCH_H__


#include "common.h"


// 查找接口
#define INVALID_INDEX           (-1)

typedef struct {
    int (*mpf_search)(int const [], int const, int const);
} search_t;


// 常用查找实现
extern int sequential_search(int const A_BUF[],
                             int const size,
                             int const KEY); // 顺序查找
extern int binary_search(int const A_BUF[],
                         int const size,
                         int const KEY); // 二分查找
#endif // __SEARCH_H__
