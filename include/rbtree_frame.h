// rbtree_frame.h: 2-3-4树的二叉树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __RBTREE_FRAME_H__
#define __RBTREE_FRAME_H__


#include "common.h"


typedef struct rbtree_frame_node rbtree_frame_node_t;


enum {
    RB_RED = 1,
    RB_BLACK = 2,
};


// ********** 红黑树框架结点 **********
struct rbtree_frame_node {
    int m_key;
    int m_color; // 结点颜色
    rbtree_frame_node_t *mp_father;
    rbtree_frame_node_t *mp_lchild;
    rbtree_frame_node_t *mp_rchild;
};
#define RBTREE_FRAME_NODE(name)         \
            rbtree_frame_node_t name = {0, NULL, NULL, NULL}

static inline void init_rbtree_frame_node(rbtree_frame_node_t *p_node,
                                          int key)
{
    ASSERT(NULL != p_node);

    p_node->m_key = key;
    p_node->m_color = RB_BLACK; // 初始2结点，黑色
    p_node->mp_father = NULL;
    p_node->mp_lchild = NULL;
    p_node->mp_rchild = NULL;

    return;
}

static inline int is_rbtree_frame_node_full(rbtree_frame_node_t *p_node)
{
    return ((NULL != p_node->mp_lchild) && (NULL != p_node->mp_rchild));
}

static inline int is_rbtree_frame_node_empty(rbtree_frame_node_t *p_node)
{
    return ((NULL == p_node->mp_lchild) && (NULL == p_node->mp_rchild));
}


// ********** 红黑树框架 **********
static inline void init_rbtree_frame(rbtree_frame_node_t *p_tree,
                                     int key)
{
    ASSERT(NULL != p_tree);

    init_rbtree_frame_node(p_tree, key);
    p_tree->m_color = RB_RED;
}

static inline
rbtree_frame_node_t *find_first_red_node(rbtree_frame_node_t *p_tree)
{
    rbtree_frame_node_t *p_rslt = p_tree;

    ASSERT(NULL != p_tree);

    return p_rslt;
}

static inline void insert_rbtree_frame_node(rbtree_frame_node_t *p_tree,
                                            rbtree_frame_node_t *p_node)
{
    rbtree_frame_node_t *p_pos = p_tree;

    ASSERT(NULL != p_tree);
    ASSERT(NULL != p_node);
    ASSERT(RB_BLACK == p_node->m_color); // 新结点是黑色

    while (NULL != p_pos) {
        if (is_rbtree_frame_node_full(p_pos)) { // 当前结点是满的
            p_pos = (p_node->m_key < p_pos->m_key) ?
                        p_pos->mp_lchild : p_pos->mp_rchild;

            continue;
        }

        if (RB_BLACK == p_pos->m_color) { // 空2结点，直接插入
            break;
        }
    }

    return;
}
#endif
