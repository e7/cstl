// config.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD__LIST_H__
#define __STD__LIST_H__


#include "iterator.h"


typedef struct ldlist_node std_ldlist_head_st, std_ldlist_node_st;
struct ldlist_node {
    std_ldlist_node_st *mp_prev;
    std_ldlist_node_st *mp_next;
};

// interface
typedef struct ldlist std_ldlist_st; // 循环双链表

#endif // __STD__LIST_H__
