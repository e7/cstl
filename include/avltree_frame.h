// avltree_frame.h: avl树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __AVLTREE_FRAME_H__
#define __AVLTREE_FRAME_H__

#include "common.h"


typedef struct avltree_frame avltree_frame_t;
struct avltree_frame {
    int m_key; // 键
    void *mp_value; // 值
    int m_balance_factor; // 平衡因子
    avltree_frame_t *mp_ftree; // 父树
    avltree_frame_t *mp_ltree; // 左子树
    avltree_frame_t *mp_rtree; // 右子树
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
    ASSERT(NULL != (*pp_tree)->mp_ltree);

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
    ASSERT(NULL != (*pp_tree)->mp_rtree);

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

// ********** avl树的内部查找 **********
typedef struct {
    avltree_frame_t **mpp_father;
    avltree_frame_t **mpp_child;
} avl_iter_t; // 上滤迭代器

// 寻找左子树的最大值
static inline
avl_iter_t *find_lmax_avltree_frame(avltree_frame_t **pp_tree,
                                    avl_iter_t *p_iter)
{
    ASSERT(NULL != pp_tree);

    // 开始寻找
    ASSERT(NULL != *pp_tree);
    ASSERT(NULL != (*pp_tree)->mp_ltree); // 存在左子树
    p_iter->mpp_father = pp_tree;
    p_iter->mpp_child = &(*pp_tree)->mp_ltree;
    while (NULL != (*p_iter->mpp_child)->mp_rtree) {
        p_iter->mpp_father = p_iter->mpp_child;
        p_iter->mpp_child = &(*p_iter->mpp_child)->mp_rtree;
    }

    return p_iter;
}

// 寻找右子树的最小值
static inline
avl_iter_t *find_rmin_avltree_frame(avltree_frame_t **pp_tree,
                                    avl_iter_t *p_iter)
{
    ASSERT(NULL != pp_tree);

    // 开始寻找
    ASSERT(NULL != *pp_tree);
    ASSERT(NULL != (*pp_tree)->mp_rtree); // 存在右子树
    p_iter->mpp_father = pp_tree;
    p_iter->mpp_child = &(*pp_tree)->mp_rtree;
    while (NULL != (*p_iter->mpp_child)->mp_ltree) {
        p_iter->mpp_father = p_iter->mpp_child;
        p_iter->mpp_child = &(*p_iter->mpp_child)->mp_ltree;
    }

    return p_iter;
}

// ********** avl树接口 **********
static inline
void init_avltree_frame(avltree_frame_t *p_tree, int key)
{
    ASSERT(NULL != p_tree);

    p_tree->m_key = key;
    p_tree->mp_value = NULL;
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
    if (NULL != *pp_vacancy) { // 已存在
        rslt = -1;
        goto FINAL;
    }

    // 执行插入
    p_subtree->mp_ftree = (*pp_orig);
    (*pp_vacancy) = p_subtree;

    // 调整
    p_child_tree = p_subtree;
    while(NULL != (*pp_orig)) {
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

static inline
avl_iter_t *find_avltree_frame(avltree_frame_t **pp_tree,
                               int key,
                               avl_iter_t *p_iter)
{
    ASSERT(NULL != pp_tree);
    ASSERT(NULL != p_iter);

    ASSERT(NULL != *pp_tree);

    p_iter->mpp_father = NULL;
    p_iter->mpp_child = pp_tree;
    while (key != (*p_iter->mpp_child)->m_key) {
        if ((*p_iter->mpp_child)->m_key < key) {
            p_iter->mpp_father = p_iter->mpp_child;
            p_iter->mpp_child = &(*p_iter->mpp_child)->mp_rtree;
        } else if ((*p_iter->mpp_child)->m_key > key) {
            p_iter->mpp_father = p_iter->mpp_child;
            p_iter->mpp_child = &(*p_iter->mpp_child)->mp_ltree;
        } else {
            ASSERT(0);
        }

        if (NULL == *p_iter->mpp_child) {
            p_iter = NULL;
            break;
        }
    }

    return p_iter;
}

static inline
int remove_avltree_frame(avltree_frame_t **pp_tree, int key)
{
    int rslt = 0;
    avl_iter_t del = {
        NULL, NULL,
    };
    avl_iter_t alternate = {
        NULL, NULL,
    };
    avl_iter_t *p_del = NULL;
    avltree_frame_t *p_static_del = NULL;

    ASSERT(NULL != pp_tree);
    ASSERT(NULL != *pp_tree);

    // 查找待删除的结点
    p_del = find_avltree_frame(pp_tree, key, &del);
    if (NULL == p_del) {
        rslt = -1;
        goto FINAL;
    }

    // 迭代寻找候补结点
    while (TRUE) {
        if (NULL != (*p_del->mpp_child)->mp_ltree) {
            alternate.mpp_father = p_del->mpp_father;
            alternate.mpp_child = p_del->mpp_child;
            p_del = find_lmax_avltree_frame(p_del->mpp_child, &del);
        } else if (NULL != (*p_del->mpp_child)->mp_rtree) {
            alternate.mpp_father = p_del->mpp_father;
            alternate.mpp_child = p_del->mpp_child;
            p_del = find_rmin_avltree_frame(p_del->mpp_child, &del);
        } else {
            break;
        }

        // 交换结点核心数据
        SWAP((*del.mpp_child)->m_key, (*alternate.mpp_child)->m_key); // 键
        SWAP((*del.mpp_child)->mp_value,
             (*alternate.mpp_child)->mp_value); // 值
    }

    // 上滤然后删除
    p_static_del = *p_del->mpp_child;
    ASSERT(NULL == (*p_del->mpp_child)->mp_ltree);
    ASSERT(NULL == (*p_del->mpp_child)->mp_rtree);

    if (NULL == p_del->mpp_father) { // 该树只有一个结点
        goto DELETE;
    }

    alternate.mpp_father = p_del->mpp_father;
    alternate.mpp_child = p_del->mpp_child;
    while (NULL != (*alternate.mpp_father)) {
        // 调整父树平衡因子
        if (*alternate.mpp_child == (*alternate.mpp_father)->mp_ltree) {
            ++(*alternate.mpp_father)->m_balance_factor;
        } else if (*alternate.mpp_child == (*alternate.mpp_father)->mp_rtree) {
            --(*alternate.mpp_father)->m_balance_factor;
        } else {
            ASSERT(0);
        }

        // 根据平衡因子执行旋转
       if (1 == ABS((*alternate.mpp_father)->m_balance_factor)) {
            // 说明原来的平衡因子是0，平衡因子为0的结点容错性更强，
            // 不必再上滤
            break;
       } else if (0 == (*alternate.mpp_father)->m_balance_factor) {
           alternate.mpp_child = alternate.mpp_father;

           if (NULL == (*alternate.mpp_father)->mp_ftree) {
               break;
           }

           if (NULL == (*alternate.mpp_father)->mp_ftree->mp_ftree) {
               alternate.mpp_father = pp_tree;
           } else if (
               (*alternate.mpp_father)->mp_ftree
                   == (*alternate.mpp_father)->mp_ftree->mp_ftree->mp_ltree)
           {
               alternate.mpp_father
                   = &((*alternate.mpp_father)->mp_ftree->mp_ftree->mp_ltree);
           }
           else if (
               (*alternate.mpp_father)->mp_ftree
                   == (*alternate.mpp_father)->mp_ftree->mp_ftree->mp_rtree)
           {
               alternate.mpp_father
                   = &((*alternate.mpp_father)->mp_ftree->mp_ftree->mp_rtree);
           } else {
               ASSERT(0);
           }
       } else if ((*alternate.mpp_father)->m_balance_factor < -1) {
           ASSERT(NULL != (*alternate.mpp_father)->mp_ltree);

           if ((*alternate.mpp_father)->mp_ltree->m_balance_factor <= 0) {
                avl_r_rotate(alternate.mpp_father);
           } else if ((*alternate.mpp_father)->mp_ltree->m_balance_factor > 0)
           {
                avl_lr_rotate(alternate.mpp_father);
           } else {
               ASSERT(0);
           }
           break;
       } else if ((*alternate.mpp_father)->m_balance_factor > 1) {
           ASSERT(NULL != (*alternate.mpp_father)->mp_rtree);

           if ((*alternate.mpp_father)->mp_rtree->m_balance_factor >= 0) {
                avl_l_rotate(alternate.mpp_father);
           } else if ((*alternate.mpp_father)->mp_rtree->m_balance_factor < 0)
           {
                avl_rl_rotate(alternate.mpp_father);
           } else {
               ASSERT(0);
           }
           break;
       } else {
           ASSERT(0);
       }
    }
    ASSERT(p_static_del == *p_del->mpp_child); // 一系列调整后仍持有

DELETE:
    (*p_del->mpp_child)->mp_ftree = NULL;
    *p_del->mpp_child = NULL;

FINAL:
    return rslt;
}
#endif // __AVLTREE_FRAME_H__
