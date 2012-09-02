// mem.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __MEM_H__
#define __MEM_H__


#include "list_frame.h"


// 支持的对象大小
static int const A_OBJ_SIZE_SURPPORT[] = {
    1 << 3, // 8
    1 << 4, // 16
    1 << 5, // 32
    1 << 6, // 64
    (1 << 6) + (1 << 5), // 96
    1 << 7, // 128
    (1 << 7) + (1 << 6), // 192
    1 << 8, // 256
    1 << 9, // 512
    1 << 10, // 1024
    1 << 11, // 2048
};
#define MAX_OBJ_SIZE    \
            (A_OBJ_SIZE_SURPPORT[ARRAY_COUNT(A_OBJ_SIZE_SURPPORT) - 1])
#define MIN_OBJ_SIZE    (A_OBJ_SIZE_SURPPORT[0])
#define OBJ_SIZE_NUM    (ARRAY_COUNT(A_OBJ_SIZE_SURPPORT))


// ******************** 对象页 ********************
#define OBJS_PER_PAGE               64

typedef struct s_obj_shell obj_shell_t;
typedef struct s_page page_t;

struct s_obj_shell {
    union {
        int m_size;
        obj_shell_t *mp_next;
    } m_intptr; // 对象大小或指向下一对象壳
    char m_obj[0];
}; // 对象壳类型

struct s_page {
    ldlist_node_t m_ldlist_node; // 页节点
    int m_use_count; // 对象使用计数
    obj_shell_t *mp_free_list; // 空闲对象壳链
    obj_shell_t *mp_objs_start; // 对象组起始地址
    obj_shell_t *mp_objs_end; // 对象组终止地址
    char ma_obj_shs[0];
}; // 对象页


// ******************** 页仓库 ********************
typedef struct {
    int m_obj_size; // 该类型页对象大小
    ldlist_head_t m_ldlist_partial; // 部分占用页
    ldlist_head_t m_ldlist_full; // 满页
    page_t *mp_page_current; // 当前页
} page_base_t;


// ******************** 内存池接口 ********************
typedef struct {
    page_base_t ma_obj_cache[OBJ_SIZE_NUM]; // 各类型页对象缓存
} mempool_t;

extern int mempool_build(mempool_t *p_mempool);
extern void *mempool_alloc(mempool_t *p_mempool, int obj_size);
extern void *mempool_array_alloc(mempool_t *p_mempool,
                                 int obj_size,
                                 int obj_count);
extern void mempool_free(mempool_t *p_mempool, void *p_obj);
extern void mempool_destroy(mempool_t *p_mempool);
#endif // __MEM_H__
