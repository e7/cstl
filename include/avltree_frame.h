// avltree_frame.h: avl树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __AVLTREE_FRAME_H__
#define __AVLTREE_FRAME_H__

#include "common.h"


typedef struct avltree_frame avltree_frame_t;
struct avltree_frame {
    int m_key; // 键
    int m_balance_factor; // 平衡因子
    avltree_frame_t *mp_ftree; // 父树
    avltree_frame_t *mp_ltree; // 左子树
    avltree_frame_t *mp_rtree; // 右子树
    int m_size; // 树结点数目
};

#define AVLTREE_FRAME_NODE(name)         \
            avltree_frame_node_t name = {0, 0, NULL, NULL, NULL}


// ********** avl树的四种旋转 **********
static inline
void avl_r_rotate(avltree_frame_t *p_tree)
{
    //        1 o                    2 o           //
    //         / \                    / \          //
    //      2 o   o 3              4 o   o 1       //
    //       / \         -->        /   / \        //
    //    4 o   o 5              6 o   o   o 3     //
    //     /                           5           //
    //  6 o                                        //

    ASSERT(NULL != p_tree);
}

static inline
void avl_l_rotate(avltree_frame_t *p_tree)
{
    ASSERT(NULL != p_tree);
}

static inline
void avl_rl_rotate(avltree_frame_t *p_tree)
{
    ASSERT(NULL != p_tree);
}

static inline
void avl_lr_rotate(avltree_frame_t *p_tree)
{
    ASSERT(NULL != p_tree);
}


// ********** avl树接口 **********
static inline
void init_avltree_frame(avltree_frame_t *p_tree, int key)
{
    ASSERT(NULL != p_tree);

    p_tree->m_key = key;
    p_tree->m_balance_factor = 0;
    p_tree->mp_ftree = NULL;
    p_tree->mp_ltree = NULL;
    p_tree->mp_rtree = NULL;
}

static inline
int insert_avltree_frame(avltree_frame_t *p_tree,
                         avltree_frame_t *p_subtree)
{
    int rslt = 0;
    int const UPWARD_COUNT = 3; // 最多影响曾祖父结点
    avltree_frame_t *p_ftree = NULL;
    avltree_frame_t **pp_vacancy = NULL;
    avltree_frame_t *p_child_tree = NULL;

    // 参数检查
    ASSERT(NULL != p_tree);
    ASSERT(NULL != p_subtree);

    // 只允许插入单结点子树
    ASSERT(0 == p_subtree->m_balance_factor);
    ASSERT((NULL == p_subtree->mp_ltree) && (NULL == p_subtree->mp_rtree));

    // 寻找插入位置
    pp_vacancy = &p_tree;
    while (NULL != *pp_vacancy) {
        p_ftree = (*pp_vacancy); // 更新父树

        if ((*pp_vacancy)->m_key < p_subtree->m_key) {
            pp_vacancy = &(*pp_vacancy)->mp_ltree;
        } else if ((*pp_vacancy)->m_key > p_subtree->m_key) {
            pp_vacancy = &(*pp_vacancy)->mp_rtree;
        } else {
            break;
        }
    }
    if (NULL != *pp_vacancy) {
        rslt = -1;
        goto FINAL;
    }

    // 执行插入
    p_subtree->mp_ftree = p_ftree;
    (*pp_vacancy) = p_subtree;

    // 调整
    p_child_tree = p_subtree;
    for (int i = 0; i < UPWARD_COUNT; ++i) {
        if (p_child_tree == p_ftree->mp_ltree) {
            --(p_ftree->m_balance_factor);
        } else if (p_child_tree == p_ftree->mp_rtree) {
            ++(p_ftree->m_balance_factor);
        } else {
            ASSERT(0);
        }

        if (p_ftree->m_balance_factor < -1) {
        } else if (p_ftree->m_balance_factor > 1) {
        } else {
            p_child_tree = p_ftree;
            p_ftree = p_ftree->mp_ftree;
        }
    }

FINAL:
    return rslt;
}

#endif // __AVLTREE_FRAME_H__
