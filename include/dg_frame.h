// dg_frame.h: frame of directed acyclic
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __DG_FRAME_H__
#define __DG_FRAME_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "common.h"


typedef struct s_graph_vertex           graph_vertex_t;
// typedef struct s_graph_vertex_cache     graph_vertex_cache_t;
typedef struct s_graph_edge             graph_edge_t;
typedef struct s_graph                  graph_t;


// 顶点
struct s_graph_vertex {
    int_t m_id; // 图结点编号
    void const *mpc_data; // 结点挂接的数据
};

static inline
void graph_vertex_init(graph_vertex_t *const THIS,
                       int_t id,
                       void const *pc_data)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_data);

    THIS->m_id = id;
    THIS->mpc_data = pc_data;
}

// 基于链表的顶点缓冲
// struct s_graph_vertex_cache {
//    graph_vertex_t m_vervex;
//    ldlist_frame_node_t m_list;
// };

// 边
struct s_graph_edge {
    int_t m_weight; // 权重
    graph_vertex_t *mp_vertex;
    graph_edge_t *mp_next;
};

static inline
void graph_edge_init(graph_edge_t *const THIS, graph_vertex_t *p_vertex)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != p_vertex);

    THIS->m_weight = 0;
    THIS->mp_vertex = p_vertex;
    THIS->mp_next = NULL;
}

static inline
void graph_edge_add_edge(graph_edge_t *const THIS, graph_edge_t *p_edge)
{
    graph_edge_t *p_tmp_edge = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL != p_edge);

    p_tmp_edge = THIS->mp_next;
    THIS->mp_next = p_edge;
    p_edge->mp_next = p_tmp_edge;
}

// 图
struct s_graph {
//    ldlist_frame_head_t m_vertex_buf; // 顶点缓冲
    graph_vertex_t *mp_vertex;
    graph_edge_t *mp_first_edge;
    graph_t *mp_next;
};


// 图接口函数
static inline
void graph_build(graph_t *const THIS, graph_vertex_t *p_vertex)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != p_vertex);

    THIS->mp_vertex = p_vertex;
    THIS->mp_first_edge = NULL;
    THIS->mp_next = NULL;
}

static inline
void graph_add_graph(graph_t *const THIS, graph_t *p_graph)
{
    graph_t *p_tmp_graph = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL != p_graph);

    p_tmp_graph = THIS->mp_next;
    THIS->mp_next = p_graph;
    p_graph->mp_next = p_tmp_graph;
}

static inline
void graph_add_edge(graph_t *const THIS, graph_edge_t *p_edge)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != p_edge);

    graph_edge_add_edge(THIS->mp_first_edge, p_edge);
}

static inline
void graph_destroy(graph_t *const THIS)
{
    ASSERT(NULL != THIS);
}


#ifdef __cplusplus
}
#endif // end of extern "C"
#endif // __DG_FRAME_H__
