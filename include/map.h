// map.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __MAP_H__
#define __MAP_H__


#include "avltree_frame.h"
#include "iterator.h"
#include "mem.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct s_map map_t;


struct s_map {
    avltree_frame_t *mp_root;
    int_t m_count; // 结点数
};


// ********** 映射接口函数 **********
static inline
void map_build(map_t *const THIS)
{
    ASSERT(NULL != THIS);

    THIS->mp_root = NULL;
    THIS->m_count = 0;
}


#ifdef __cplusplus
}
#endif // end of extern "C"
#endif // __MAP_H__
