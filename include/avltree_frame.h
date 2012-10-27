// avltree_frame.h: avl树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __AVLTREE_FRAME_H__
#define __AVLTREE_FRAME_H__

#include "common.h"


typedef struct avltree_frame avltree_frame_t;
struct avltree_frame {
    int m_key; // 键
    int m_height; // 树高度
    avltree_frame_t *mp_ftree; // 父树
    avltree_frame_t *mp_ltree; // 左子树
    avltree_frame_t *mp_rtree; // 右子树
    int m_size; // 树结点数目
};

#define AVLTREE_FRAME_NODE(name)         \
            avltree_frame_node_t name = {0, 0, NULL, NULL, NULL}

// ********** avl树接口 **********
static inline
void init_avltree_frame(avltree_frame_t *p_tree, int key)
{
    ASSERT(NULL != p_tree);

    p_tree->m_key = key;
    p_tree->m_height = 0;
    p_tree->mp_ftree = NULL;
    p_tree->mp_ltree = NULL;
    p_tree->mp_rtree = NULL;
}

static inline
int insert_avltree_frame(avltree_frame_t *p_tree,
                         avltree_frame_t *p_subtree)
{
    int rslt = 0;
    avltree_frame_t **pp_father = NULL;

    ASSERT(NULL != p_tree);
    ASSERT(NULL != p_subtree);

    // 执行bst插入
    pp_father = &p_tree;
    while (NULL != *pp_father) {
        if ((*pp_father)->m_key < p_subtree->m_key) {
            pp_father = &(*pp_father)->mp_ltree;
        } else if ((*pp_father)->m_key > p_subtree->m_key) {
            pp_father = &(*pp_father)->mp_rtree;
        } else {
            break;
        }
    }
    if (NULL != *pp_father) {
        rslt = -1;
        goto FINAL;
    }
    (*pp_father) = p_subtree;

    // 调整

FINAL:
    return rslt;
}

#endif // __AVLTREE_FRAME_H__
