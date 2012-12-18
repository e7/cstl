// list_frame.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __LIST_FRAME_H__
#define __LIST_FRAME_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


// ******************** 循环双链表框架 ********************
typedef struct ldlist_frame_node ldlist_frame_node_t, ldlist_frame_head_t;
struct ldlist_frame_node {
    ldlist_frame_node_t *mp_prev;
    ldlist_frame_node_t *mp_next;
};

// 结点初始化，需自旋
#define LDLIST_FRAME_HEAD_INIT(name)        {&(name), &(name)}
#define LDLIST_FRAME_HEAD(name)             \
            ldlist_frame_head_t name = LDLIST_FRAME_HEAD_INIT(name)
static inline void init_ldlist_frame_node(ldlist_frame_node_t *p_node)
{
    ASSERT(NULL != p_node);

    p_node->mp_prev = p_node;
    p_node->mp_next = p_node;
}

// 结点是否落单
static inline int_t ldlist_frame_node_alone(ldlist_frame_node_t const *p_node)
{
    int_t rslt = FALSE;

    ASSERT (NULL != p_node);

    if (p_node == p_node->mp_prev) {
        ASSERT(p_node == p_node->mp_next);
        rslt = TRUE;
    }
    if (p_node == p_node->mp_next) {
        ASSERT(p_node == p_node->mp_prev);
        rslt = TRUE;
    }

    return rslt;
}

// 向链表添加结点
static inline void ldlist_frame_add(ldlist_frame_node_t *p_new_node,
                                    ldlist_frame_node_t *p_prev_node,
                                    ldlist_frame_node_t *p_next_node)
{
    ASSERT(NULL != p_new_node);
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);

    p_next_node->mp_prev = p_new_node;
    p_new_node->mp_next = p_next_node;
    p_new_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_new_node;
}

static inline void ldlist_frame_add_head(ldlist_frame_head_t *p_head,
                                         ldlist_frame_node_t *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    ldlist_frame_add(p_node, p_head, p_head->mp_next);
}

static inline void ldlist_frame_add_tail(ldlist_frame_head_t *p_head,
                                         ldlist_frame_node_t *p_node)
{
    ASSERT(NULL != p_head);
    ASSERT(NULL != p_node);

    ldlist_frame_add(p_node, p_head->mp_prev, p_head);
}

// 访问链表结点数据
static inline
ldlist_frame_node_t *ldlist_frame_first(ldlist_frame_head_t const *pc_list)
{
    ASSERT(NULL != pc_list);

    return pc_list->mp_next;
}

static inline
ldlist_frame_node_t *ldlist_frame_prev(ldlist_frame_node_t const *pc_node)
{
    ASSERT(NULL != pc_node);

    return pc_node->mp_prev;
}

static inline
ldlist_frame_node_t *ldlist_frame_next(ldlist_frame_node_t const *pc_node)
{
    ASSERT(NULL != pc_node);

    return pc_node->mp_next;
}

static inline
ldlist_frame_node_t *ldlist_frame_last(ldlist_frame_head_t const *pc_list)
{
    ASSERT(NULL != pc_list);

    return pc_list->mp_prev;
}

#define LDLIST_FRAME_ENTRY(ptr, type, member)         \
            CONTAINER_OF(ptr, type, member)
#define LDLIST_FRAME_FOR_EACH(p_pos_node, p_head_node)        \
            for ((p_pos_node) = (p_head_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_pos_node)->mp_next)
#define LDLIST_FRAME_FOR_EACH_SAFE(p_pos_node, p_cur_next, p_head_node)   \
            for ((p_pos_node) = (p_head_node)->mp_next, \
                     (p_cur_next) = (p_pos_node)->mp_next; \
                 (p_pos_node) != (p_head_node); \
                 (p_pos_node) = (p_cur_next), \
                     (p_cur_next) = (p_pos_node)->mp_next)


// 从链表删除结点
static inline void ldlist_frame_del_orig(ldlist_frame_node_t *p_prev_node,
                                         ldlist_frame_node_t *p_next_node)
{
    ASSERT(NULL != p_prev_node);
    ASSERT(NULL != p_next_node);
    ASSERT(p_prev_node->mp_next == p_next_node->mp_prev);

    // 自旋
    p_next_node->mp_prev = p_prev_node;
    p_prev_node->mp_next = p_next_node;
}

static inline void ldlist_frame_del(ldlist_frame_node_t *p_node)
{
    ASSERT(NULL != p_node);

    ldlist_frame_del_orig(p_node->mp_prev, p_node->mp_next);
}


#ifdef __cplusplus
}
#endif // end of extern "C"
#endif // __LIST_FRAME_H__
