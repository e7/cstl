// avltree_frame.h: 2-3-4树的二叉树实现。
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __AVLTREE_FRAME_H__
#define __AVLTREE_FRAME_H__

#include "common.h"


typedef struct avltree_frame_node avltree_frame_node_t;

struct avltree_frame_node {
    int m_height; // 结点高度
    avltree_frame_node_t *mp_father; // 父结点
    avltree_frame_node_t *mp_lchild; // 左儿子
    avltree_frame_node_t *mp_rchild; // 右儿子
};
#endif // __AVLTREE_FRAME_H__
