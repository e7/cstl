// list_frame.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD__LIST_H__
#define __STD__LIST_H__


#include "common.h"


// ******************** 循环双链表框架 ********************
typedef struct ldlist_node ldlist_node_t;
typedef struct ldlist_node ldlist_head_t;
struct ldlist_node {
    ldlist_node_t *mp_prev;
    ldlist_node_t *mp_next;
};

// 结点初始化
#define LDLIST_HEAD_INIT(name)        {&(name), &(name)}
#define LDLIST_HEAD(name)             \
            ldlist_head_t name = LDLIST_HEAD_INIT(name)
static inline void init_ldlist_node(ldlist_node_t *p_node)
{
    ASSERT(NULL != p_node);

    p_node->mp_prev = p_node;
    p_node->mp_next = p_node;
}

// 结点是否落单
static inline int ldlist_node_alone(ldlist_node_t const *p_node)
{
    ASSERT (NULL != p_node);

    return (p_node->mp_prev == p_node->mp_next);
}

// 向链表添加结点
static inline void ldlist_add(ldlist_node_t *p_new_node,
                              ldlist_node_t *p_prev_node,
                              ldlist_node_t *p_next_node)
{
    ASSERT(NULL != p_new_node);
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);

    p_next_node->mp_prev = p_new_node;
    p_new_node->mp_next = p_next_node;
    p_new_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_new_node;
}

static inline void ldlist_add_head(ldlist_head_t *p_head,
                                   ldlist_node_t *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    ldlist_add(p_node, p_head, p_head->mp_next);
}

static inline void ldlist_add_tail(ldlist_head_t *p_head,
                                   ldlist_node_t *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    ldlist_add(p_node, p_head->mp_prev, p_head);
}

// 访问链表结点数据
static inline ldlist_head_t *ldlist_head(ldlist_head_t const *pc_list)
{
    ldlist_head_t *p_rslt = NULL;

    ASSERT(NULL != pc_list);

    ASSERT(NULL == p_rslt);
    if (!ldlist_node_alone(pc_list)) {
        p_rslt = pc_list->mp_next;
    }

    return p_rslt;
}

static inline ldlist_head_t *ldlist_tail(ldlist_head_t const *pc_list)
{
    ldlist_head_t *p_rslt = NULL;

    ASSERT(NULL != pc_list);

    ASSERT(NULL == p_rslt);
    if (!ldlist_node_alone(pc_list)) {
        p_rslt = pc_list->mp_prev;
    }

    return p_rslt;
}

#define LDLIST_ENTRY(ptr, type, member)         \
            CONTAINER_OF(ptr, type, member)
#define LDLIST_FOR_EACH(p_pos_node, p_head_node)        \
            for ((p_pos_node) = (p_head_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_pos_node)->mp_next)
#define LDLIST_FOR_EACH_SAFE(p_pos_node, p_cur_next, p_head_node)   \
            for ((p_pos_node) = (p_head_node)->mp_next, \
                     (p_cur_next) = (p_pos_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_cur_next), \
                     (p_cur_next) = (p_pos_node)->mp_next)


// 从链表删除结点
static inline void ldlist_del_orig(ldlist_node_t *p_prev_node,
                                   ldlist_node_t *p_next_node)
{
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);
    ASSERT(p_prev_node->mp_next == p_next_node->mp_prev);

    // 自旋
    p_next_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_next_node;
}

static inline void ldlist_del(ldlist_node_t *p_node)
{
    ASSERT(NULL != p_node);

    ldlist_del_orig(p_node->mp_prev, p_node->mp_next);
}

static inline ldlist_node_t *ldlist_del_head(ldlist_node_t *p_list)
{
    ldlist_node_t *p_rslt = NULL;

    ASSERT(NULL != p_list);

    p_rslt = ldlist_head(p_list);
    if (NULL != p_rslt) {
        ldlist_del(p_rslt);
    }

    return p_rslt;
}

static inline ldlist_node_t *ldlist_del_tail(ldlist_node_t *p_list)
{
    ldlist_node_t *p_rslt = NULL;

    ASSERT(NULL != p_list);

    p_rslt = ldlist_tail(p_list);
    if (NULL != p_rslt) {
        ldlist_del(p_rslt);
    }

    return p_rslt;
}
#endif // __STD__LIST_H__
