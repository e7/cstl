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
void avl_r_rotate(avltree_frame_t **pp_tree)
{
    //        1 o                    2 o           //
    //         / \                    / \          //
    //      2 o   o 3              4 o   o 1       //
    //       / \         -->        /   / \        //
    //    4 o   o 5              6 o   o   o 3     //
    //     /                           5           //
    //  6 o                                        //

    ASSERT(NULL != pp_tree);
    ASSERT(NULL != (*pp_tree));

    // 旋转
    do {
        avltree_frame_t *p_father= (*pp_tree)->mp_ftree;
        avltree_frame_t *p_old_root = (*pp_tree);
        avltree_frame_t *p_new_root = (*pp_tree)->mp_ltree;
        avltree_frame_t *p_child = (*pp_tree)->mp_ltree->mp_rtree;

        // start ratate
        p_old_root->mp_ftree = p_new_root;
        (*pp_tree) = p_new_root;

        p_new_root->mp_ftree = p_father;
        p_old_root->mp_ltree = p_new_root->mp_rtree;

        p_new_root->mp_rtree = p_old_root;
        if (NULL != p_child) {
            p_child->mp_ftree = p_old_root;
        }

        // 修正平衡因子
        p_old_root->m_balance_factor
            += MAX(0 - p_new_root->m_balance_factor, 0) + 1;
        p_new_root->m_balance_factor
            += MAX(0, p_old_root->m_balance_factor) + 1;
    } while (0);

    return;
}

static inline
void avl_l_rotate(avltree_frame_t **pp_tree)
{
    ASSERT(NULL != pp_tree);
    ASSERT(NULL != (*pp_tree));

    do {
        avltree_frame_t *p_father= (*pp_tree)->mp_ftree;
        avltree_frame_t *p_old_root = (*pp_tree);
        avltree_frame_t *p_new_root = (*pp_tree)->mp_rtree;
        avltree_frame_t *p_child = (*pp_tree)->mp_rtree->mp_ltree;

        // start ratate
        p_old_root->mp_ftree = p_new_root;
        (*pp_tree) = p_new_root;

        p_new_root->mp_ftree = p_father;
        p_old_root->mp_rtree = p_new_root->mp_ltree;

        p_new_root->mp_ltree = p_old_root;
        if (NULL != p_child) {
            p_child->mp_ftree = p_old_root;
        }

        // 修正平衡因子
        p_old_root->m_balance_factor
            -= MAX(0, p_new_root->m_balance_factor) + 1;
        p_new_root->m_balance_factor
            += MIN(p_old_root->m_balance_factor, 0) - 1;
    } while (0);

    return;
}

static inline
void avl_lr_rotate(avltree_frame_t **pp_tree)
{
    ASSERT(NULL != pp_tree);
    ASSERT(NULL != (*pp_tree));

    // 执行双旋转
    avl_l_rotate(&((*pp_tree)->mp_ltree));
    avl_r_rotate(pp_tree);

    return;
}

static inline
void avl_rl_rotate(avltree_frame_t **pp_tree)
{
    ASSERT(NULL != pp_tree);
    ASSERT(NULL != (*pp_tree));

    // 执行双旋转
    avl_r_rotate(&((*pp_tree)->mp_rtree));
    avl_l_rotate(pp_tree);

    return;
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
int insert_avltree_frame(avltree_frame_t **pp_tree,
                         avltree_frame_t *p_subtree)
{
    int rslt = 0;
    int const UPWARD_COUNT = 3; // 最多影响曾祖父结点
    avltree_frame_t **pp_orig = NULL; // 树根指针真身
    avltree_frame_t **pp_vacancy = NULL; //待插位置
    avltree_frame_t *p_child_tree = NULL;

    // 参数检查
    ASSERT(NULL != pp_tree);
    ASSERT(NULL != p_subtree);
    ASSERT(0 == p_subtree->m_balance_factor);
    ASSERT((NULL == p_subtree->mp_ltree) && (NULL == p_subtree->mp_rtree));

    // 寻找插入位置
    pp_vacancy = pp_tree;
    pp_orig = pp_tree;
    while (NULL != *pp_vacancy) {
        pp_orig = pp_vacancy;

        if ((*pp_vacancy)->m_key > p_subtree->m_key) {
            pp_vacancy = &(*pp_vacancy)->mp_ltree;
        } else if ((*pp_vacancy)->m_key < p_subtree->m_key) {
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
    p_subtree->mp_ftree = (*pp_orig);
    (*pp_vacancy) = p_subtree;

    // 调整
    p_child_tree = p_subtree;
    for (int i = 0; i < UPWARD_COUNT; ++i) {
        if (NULL == (*pp_orig)) {
            break;
        }

        if (p_child_tree == (*pp_orig)->mp_ltree) {
            --((*pp_orig)->m_balance_factor);
        } else if (p_child_tree == (*pp_orig)->mp_rtree) {
            ++((*pp_orig)->m_balance_factor);
        } else {
            ASSERT(p_child_tree == (*pp_orig));

            break;
        }

        if ((*pp_orig)->m_balance_factor < -1) {
            if ((*pp_orig)->mp_ltree->m_balance_factor < 0) {
                avl_r_rotate(pp_orig);
            } else if ((*pp_orig)->mp_ltree->m_balance_factor > 0) {
                avl_lr_rotate(pp_orig);
            } else {
                ASSERT(0);
            }

            break;
        } else if ((*pp_orig)->m_balance_factor > 1) {
            if ((*pp_orig)->mp_rtree->m_balance_factor > 0) {
                avl_l_rotate(pp_orig);
            } else if ((*pp_orig)->mp_rtree->m_balance_factor < 0) {
                avl_rl_rotate(pp_orig);
            } else {
                ASSERT(0);
            }

            break;
        } else if (0 == (*pp_orig)->m_balance_factor) {
            break;
        } else {
            p_child_tree = (*pp_orig);
            if (NULL == (*pp_orig)->mp_ftree) {
                break;
            }
            if (NULL == (*pp_orig)->mp_ftree->mp_ftree) {
                pp_orig = pp_tree;
            } else if ((*pp_orig)->mp_ftree
                           == (*pp_orig)->mp_ftree->mp_ftree->mp_ltree)
            {
                pp_orig = &((*pp_orig)->mp_ftree->mp_ftree->mp_ltree);
            } else if ((*pp_orig)->mp_ftree
                           == (*pp_orig)->mp_ftree->mp_ftree->mp_rtree)
            {
                pp_orig = &((*pp_orig)->mp_ftree->mp_ftree->mp_rtree);
            } else {
                ASSERT(0);
            }
        }
    }

FINAL:
    return rslt;
}

#endif // __AVLTREE_FRAME_H__
