// avltree_frame.h: 2-3-4树的二叉树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __AVLTREE_FRAME_H__
#define __AVLTREE_FRAME_H__

#include "common.h"


typedef struct avltree_frame_node avltree_frame_node_t;

struct avltree_frame_node {
    int m_key; // 键
    int m_height; // 结点高度
    avltree_frame_node_t *mp_father; // 父结点
    avltree_frame_node_t *mp_lchild; // 左儿子
    avltree_frame_node_t *mp_rchild; // 右儿子
};

#define AVLTREE_FRAME_NODE(name)         \
            avltree_frame_node_t name = {0, 0, NULL, NULL, NULL}

static inline
void init_avltree_frame_node_orig(avltree_frame_node_t *p_node, int key)
{
    ASSERT(NULL != p_node);

    p_node->m_key = key;
    p_node->m_height = 0;
    p_node->mp_father = NULL;
    p_node->mp_lchild = NULL;
    p_node->mp_rchild = NULL;
}
#endif // __AVLTREE_FRAME_H__
