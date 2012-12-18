// list_frame.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __LIST_H__
#define __LIST_H__


#include "list_frame.h"
#include "iterator.h"
#include "mem.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct ldlist_node ldlist_node_t;
typedef struct ldlist_iterator ldlist_iterator_t;
typedef struct ldlist ldlist_t;


// 循环双链表结点
struct ldlist_node {
    void *mp_data;
    ldlist_frame_node_t m_node;
};

// 循环双链表迭代器
struct ldlist_iterator {
    ldlist_frame_node_t *mp_node; // 结点
    iterator_t m_iter; // 迭代器接口
    int_t m_element_size; // 元素大小
};

// 循环双链表
struct ldlist {
    mempool_t *mp_mempool;
    ldlist_frame_head_t m_head;
    int_t m_element_size; // 元素大小
    ldlist_iterator_t m_begin;
    ldlist_iterator_t m_end;
};


// ******************** 循环双链表接口 ********************
extern void ldlist_build(ldlist_t *const THIS,
                         mempool_t *p_mempool,
                         int_t element_size);

extern int_t ldlist_push_front(ldlist_t *const THIS, void const *pc_data);

extern int_t ldlist_push_back(ldlist_t *const THIS, void const *pc_data);

extern int_t ldlist_is_empty(ldlist_t *const THIS);

extern void *ldlist_first(ldlist_t *const THIS);

extern void *ldlist_last(ldlist_t *const THIS);

extern void ldlist_pop_front(ldlist_t *const THIS);

extern void ldlist_pop_back(ldlist_t *const THIS);

extern iterator_t *ldlist_begin(ldlist_t *const THIS,
                                ldlist_iterator_t *p_iter);

extern iterator_t const *ldlist_end(ldlist_t *const THIS);

extern void ldlist_clean(ldlist_t *const THIS);

extern void ldlist_destroy(ldlist_t *const THIS);


#ifdef __cplusplus
}
#endif // end of extern "C"
#endif // __LIST_H__
