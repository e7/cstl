// rbtree_frame.h: 2-3-4树的二叉树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __RBTREE_FRAME_H__
#define __RBTREE_FRAME_H__


#include "common.h"


typedef struct rbtree_frame_node rbtree_frame_node_t;
typedef struct rbtree_frame rbtree_frame_t;


enum {
    RB_BLACK = 0,
    RB_RED = 1,
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
            rbtree_frame_node_t name = {0, RB_BLACK, NULL, NULL, NULL}

static inline void init_rbtree_frame_node_orig(rbtree_frame_node_t *p_node,
                                               int key,
                                               int color)
{
    ASSERT(NULL != p_node);

    p_node->m_key = key;
    p_node->m_color = color;
    p_node->mp_father = NULL;
    p_node->mp_lchild = NULL;
    p_node->mp_rchild = NULL;

    return;
}

static inline void init_rbtree_frame_node(rbtree_frame_node_t *p_node,
                                          int key)
{
    ASSERT(NULL != p_node);

    init_rbtree_frame_node_orig(p_node, key, RB_BLACK);

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
struct rbtree_frame {
    rbtree_frame_node_t *mp_root;
    int m_size;
};

static inline void init_rbtree_frame(rbtree_frame_t *p_tree)
{
    ASSERT(NULL != p_tree);

    p_tree->mp_root = NULL;
    p_tree->m_size = 0;

    return;
}

static inline int is_rbtree_frame_empty(rbtree_frame_t *p_tree)
{
    ASSERT(NULL != p_tree);

    return (NULL == p_tree->mp_root);
}

static inline
rbtree_frame_node_t **find_first_red_node(rbtree_frame_t *p_tree,
                                          int key)
{
    rbtree_frame_node_t **pp_rslt = NULL;

    ASSERT(NULL != p_tree);

    pp_rslt = &p_tree->mp_root;
    while (RB_RED != (*pp_rslt)->m_color) {
        if (key < ((*pp_rslt)->m_key)) {
            pp_rslt = &(*pp_rslt)->mp_lchild;
        } else {
            pp_rslt = &(*pp_rslt)->mp_rchild;
        }

        ASSERT(NULL != pp_rslt);
        ASSERT(NULL != *pp_rslt);
    }

    return pp_rslt;
}

static inline rbtree_frame_node_t *find_rbtree_frame(rbtree_frame_t *p_tree,
                                                     int key)
{
    rbtree_frame_node_t *p_pos = NULL;

    ASSERT(NULL != p_tree);

    p_pos = p_tree->mp_root;
    while (NULL != p_pos) {
        if (key == p_pos->m_key) {
            break;
        }

        if (key < p_pos->m_key) {
            p_pos = p_pos->mp_lchild;
        } else {
            p_pos = p_pos->mp_rchild;
        }
    }

    return p_pos;
}

static inline void insert_rbtree_frame(rbtree_frame_t *p_tree,
                                       rbtree_frame_node_t *p_node)
{
    rbtree_frame_node_t **pp_father = NULL;

    ASSERT(NULL != p_tree);
    ASSERT(NULL != p_node);
    ASSERT(RB_BLACK == p_node->m_color); // 新结点是黑色

    // ***** 红黑树是空的 *****
    if (is_rbtree_frame_empty(p_tree)) {
        p_node->m_color = RB_RED;
        p_tree->mp_root = p_node;

        goto FINAL;
    }

    // ********** 结点已存在 **********
    if (NULL != find_rbtree_frame(p_tree, p_node->m_key)) {
        goto FINAL;
    }

    // ********** 结点不存在，执行插入 **********
    pp_father = find_first_red_node(p_tree, p_node->m_key);
    ASSERT(NULL != pp_father);

    // ***** red node IS empty! *****
    if (is_rbtree_frame_node_empty(*pp_father)) {
        if ((p_node->m_key) < ((*pp_father)->m_key)) {
            (*pp_father)->mp_lchild = p_node;
            (*pp_father)->mp_lchild->mp_father = (*pp_father);
        } else {
            (*pp_father)->mp_rchild = p_node;
            (*pp_father)->mp_rchild->mp_father = (*pp_father);
        }

        goto FINAL;
    }

    // ***** red node IS NOT empty! *****
    // 插入结点
    if ((p_node->m_key) < ((*pp_father)->m_key)) {
        if (NULL == (*pp_father)->mp_lchild) {
            (*pp_father)->mp_lchild = p_node;
            (*pp_father)->mp_lchild->mp_father = (*pp_father);
        } else {
            rbtree_frame_node_t *p_new_father = NULL;
            rbtree_frame_node_t *p_loser = NULL;

            if ((p_node->m_key) < ((*pp_father)->mp_lchild->m_key)) {
                p_new_father = (*pp_father)->mp_lchild;
                p_loser = p_node;
            } else {
                p_new_father = p_node;
                p_loser = (*pp_father)->mp_lchild;
            }
            ASSERT(NULL == p_new_father->mp_lchild);
            ASSERT(NULL == p_new_father->mp_rchild);
            ASSERT(NULL == p_loser->mp_lchild);
            ASSERT(NULL == p_loser->mp_rchild);

            p_new_father->mp_father = (*pp_father)->mp_father;
            p_new_father->mp_lchild = p_loser;
            p_new_father->mp_rchild = (*pp_father);

            p_new_father->mp_lchild->mp_father = p_new_father;
            p_new_father->mp_lchild->mp_lchild = NULL;
            p_new_father->mp_lchild->mp_rchild = NULL;

            p_new_father->mp_rchild->mp_father = p_new_father;
            p_new_father->mp_rchild->mp_lchild = NULL;
            p_new_father->mp_rchild->mp_rchild = NULL;

            (*pp_father) = p_new_father;
        }
    } else {
        if (NULL == (*pp_father)->mp_rchild) {
            (*pp_father)->mp_rchild = p_node;
            (*pp_father)->mp_rchild->mp_father = (*pp_father);
        } else {
            rbtree_frame_node_t *p_new_father = NULL;
            rbtree_frame_node_t *p_loser = NULL;

            if ((p_node->m_key) < ((*pp_father)->mp_rchild->m_key)) {
                p_new_father = p_node;
                p_loser = (*pp_father)->mp_rchild;
            } else {
                p_new_father = (*pp_father)->mp_rchild;
                p_loser = p_node;
            }
            ASSERT(NULL == p_new_father->mp_lchild);
            ASSERT(NULL == p_new_father->mp_rchild);
            ASSERT(NULL == p_loser->mp_lchild);
            ASSERT(NULL == p_loser->mp_rchild);

            p_new_father->mp_father = (*pp_father)->mp_father;
            p_new_father->mp_lchild = (*pp_father);
            p_new_father->mp_rchild = p_loser;

            p_new_father->mp_lchild->mp_father = p_new_father;
            p_new_father->mp_lchild->mp_lchild = NULL;
            p_new_father->mp_lchild->mp_rchild = NULL;

            p_new_father->mp_rchild->mp_father = p_new_father;
            p_new_father->mp_rchild->mp_lchild = NULL;
            p_new_father->mp_rchild->mp_rchild = NULL;

            (*pp_father) = p_new_father;
        }
    }

    // 升级为4叉结点，更新结点颜色
    (*pp_father)->m_color = RB_BLACK;
    (*pp_father)->mp_lchild->m_color = RB_RED;
    (*pp_father)->mp_rchild->m_color = RB_RED;

FINAL:
    return;
}
#endif
