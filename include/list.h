// config.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD__LIST_H__
#define __STD__LIST_H__


#include "iterator.h"


// ********** ********** 循环双链表框架 ********** **********
typedef struct ldlist_node std_ldlist_head_st, std_ldlist_node_st;
struct ldlist_node {
    std_ldlist_node_st *mp_prev;
    std_ldlist_node_st *mp_next;
};

// 结点初始化
#define STD_LDLIST_HEAD_INIT(name)        {&(name), &(name)}
#define STD_LDLIST_HEAD(name)             \
            std_ldlist_head_st name = STD_LDLIST_HEAD_INIT(name)
static inline void std_init_ldlist_node(std_ldlist_node_st *p_node)
{
    ASSERT(NULL != p_node);

    p_node->mp_prev = p_node;
    p_node->mp_next = p_node;
}

// 结点是否落单
static inline int std_ldlist_node_alone(std_ldlist_node_st const *p_node)
{
    ASSERT (NULL != p_node);

    return p_node->mp_prev == p_node->mp_next;
}

// 向链表添加结点
static inline void std_ldlist_add(std_ldlist_node_st *p_new_node,
                                  std_ldlist_node_st *p_prev_node,
                                  std_ldlist_node_st *p_next_node)
{
    ASSERT(NULL != p_new_node);
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);

    p_next_node->mp_prev = p_new_node;
    p_new_node->mp_next = p_next_node;
    p_new_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_new_node;
}

static inline void std_ldlist_add_head(std_ldlist_head_st *p_head,
                                       std_ldlist_node_st *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    std_ldlist_add(p_node, p_head, p_head->mp_next);
}

static inline void std_ldlist_add_tail(std_ldlist_head_st *p_head,
                                       std_ldlist_node_st *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    std_ldlist_add(p_node, p_head->mp_prev, p_head);
}

// 从链表删除结点
static inline void std_ldlist_del_orig(std_ldlist_node_st *p_prev_node,
                                       std_ldlist_node_st *p_next_node)
{
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);
    ASSERT(p_prev_node->mp_next == p_next_node->mp_prev);

    p_next_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_next_node;
}

static inline void std_ldlist_del(std_ldlist_node_st *p_node)
{
    ASSERT(NULL != p_node);

    std_ldlist_del_orig(p_node->mp_prev, p_node->mp_next);
}


// 访问链表结点数据
#define STD_LDLIST_ENTRY(ptr, type, member)     \
            CONTAINER_OF(ptr, type, member)
#define STD_LDLIST_FOR_EACH(p_pos_node, p_head_node)          \
            for ((p_pos_node) = (p_head_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_pos_node)->mp_next)
#define STD_LDLIST_FOR_EACH_SAFE(p_pos_node, p_cur_next, p_head_node)   \
            for ((p_pos_node) = (p_head_node)->mp_next, \
                     (p_cur_next) = (p_pos_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_cur_next), \
                     (p_cur_next) = (p_pos_node)->mp_next)


// ********** ********** 循环双链表接口 ********** **********
typedef struct ldlist std_ldlist_st; // 循环双链表

void std_init_ldlist(std_ldlist_st *const THIS);

int std_ldlist_push_front(std_ldlist_st *const THIS, void const *pc_data);

int std_ldlist_push_back(std_ldlist_st *const THIS, void const *pc_data);

void *std_ldlist_pop_front(std_ldlist_st *const THIS);

void *std_ldlist_pop_back(std_ldlist_st *const THIS);

iterator_t *std_ldlist_begin(std_ldlist_st *const THIS);

iterator_t *std_ldlist_end(std_ldlist_st *const THIS);

void std_uninit_ldlist(std_ldlist_st *const THIS);


#endif // __STD__LIST_H__
