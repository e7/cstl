#include "list.h"




// ******************** 迭代器实现 ********************
static void *ldlist_iterator_get_data(iterator_t const *pc_iter)
{
    void *p_rslt = NULL;
    ldlist_iterator_t const *pc_ldlist_iter = NULL;
    ldlist_node_t const *pc_ldlist_node = NULL;

    if (NULL == pc_iter) {
        ASSERT(NULL == p_rslt);
        goto FINAL;
    }

    pc_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    pc_ldlist_node
        = CONTAINER_OF(pc_ldlist_iter->mp_node, ldlist_node_t, m_node);
    p_rslt = pc_ldlist_node->mp_data;

FINAL:
    return p_rslt;
}

static void ldlist_iterator_set_data(iterator_t const *pc_iter,
                                     void const *pc_data)
{
    ldlist_iterator_t const *pc_ldlist_iter = NULL;
    ldlist_node_t const *pc_ldlist_node = NULL;

    if (NULL == pc_iter) {
        goto FINAL;
    }

    pc_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    pc_ldlist_node
        = CONTAINER_OF(pc_ldlist_iter->mp_node, ldlist_node_t, m_node);
    (void)memcpy(pc_ldlist_node->mp_data,
                 pc_data,
                 pc_ldlist_iter->m_element_size);

FINAL:
    return;
}

int ldlist_iterator_equal(iterator_t const *pc_iter1,
                          iterator_t const *pc_iter2)
{
    int rslt = FALSE;
    ldlist_iterator_t const *pc_ldlist_iter1 = NULL;
    ldlist_iterator_t const *pc_ldlist_iter2 = NULL;

    if (NULL == pc_iter1) {
        ASSERT(FALSE == rslt);
        goto FINAL;
    }

    if (NULL == pc_iter2) {
        ASSERT(FALSE == rslt);
        goto FINAL;
    }

    pc_ldlist_iter1 = CONTAINER_OF(pc_iter1, ldlist_iterator_t, m_iter);
    pc_ldlist_iter2 = CONTAINER_OF(pc_iter2, ldlist_iterator_t, m_iter);

    rslt = (pc_ldlist_iter1->mp_node == pc_ldlist_iter2->mp_node)
               ? TRUE : FALSE;

FINAL:
    return rslt;
}

iterator_t *ldlist_iterator_get_next(iterator_t const *pc_iter)
{
    iterator_t *p_rslt = NULL;
    ldlist_iterator_t *p_ldlist_iter = NULL;

    if (NULL == pc_iter) {
        ASSERT(NULL == p_rslt);
        goto FINAL;
    }

    p_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    p_ldlist_iter->mp_node = ldlist_frame_next(p_ldlist_iter->mp_node);
    p_rslt = (iterator_t *)pc_iter;

FINAL:
    return p_rslt;
}

iterator_t *ldlist_iterator_get_prev(iterator_t const *pc_iter)
{
    iterator_t *p_rslt = NULL;

    if (NULL == pc_iter) {
        ASSERT(NULL == p_rslt);
        goto FINAL;
    }

FINAL:
    return p_rslt;
}


// ******************** 循环双链表接口实现 ********************
int ldlist_build(ldlist_t *const THIS,
                 mempool_t *p_mempool,
                 int element_size)
{
    const ldlist_iterator_t LDLIST_ITER_TPLT = {
        &THIS->m_head,
        {
            &ldlist_iterator_get_data,
            &ldlist_iterator_set_data,
            &ldlist_iterator_equal,
            &ldlist_iterator_get_next,
            &ldlist_iterator_get_prev,
        },
        element_size,
    };
    int rslt = 0;

    if (NULL == THIS) {
        rslt = -1;

        goto FINAL;
    }

    if (NULL == p_mempool) {
        rslt = -1;

        goto FINAL;
    }

    THIS->mp_mempool = p_mempool;
    init_ldlist_frame_node(&THIS->m_head);
    THIS->m_element_size = element_size;

    memcpy(&THIS->m_begin, &LDLIST_ITER_TPLT, sizeof(LDLIST_ITER_TPLT));
    memcpy(&THIS->m_end, &LDLIST_ITER_TPLT, sizeof(LDLIST_ITER_TPLT));

FINAL:
    return rslt;
}

int ldlist_push_front(ldlist_t *const THIS, void const *pc_data)
{
    int rslt = 0;
    ldlist_node_t *p_node = NULL;

    /* 条件检查 */
    if (NULL == THIS) {
        rslt = -1;
        goto FINAL;
    }

    if (NULL == pc_data) {
        rslt = -1;
        goto FINAL;
    }

    if (NULL == THIS->mp_mempool) {
        rslt = -1;
        goto FINAL;
    }

    /* 构造结点 */
    p_node = MEMPOOL_ALLOC(THIS->mp_mempool, sizeof(ldlist_node_t));
    p_node->mp_data = MEMPOOL_ALLOC(THIS->mp_mempool, THIS->m_element_size);
    (void)memcpy(p_node->mp_data, pc_data, THIS->m_element_size);

    /* 伤一敌可连其百 */
    ldlist_frame_add_head(&THIS->m_head, &p_node->m_node);

FINAL:
    return rslt;
}

iterator_t *ldlist_begin(ldlist_t *const THIS)
{
    iterator_t *p_rslt = NULL;

    if (NULL == THIS) {
        ASSERT(NULL == p_rslt);
        goto FINAL;
    }

    // 更新
    THIS->m_begin.mp_node = ldlist_frame_first(&THIS->m_head);
    p_rslt = &THIS->m_begin.m_iter;

FINAL:
    return p_rslt;
}

iterator_t *ldlist_end(ldlist_t *const THIS)
{
    iterator_t *p_rslt = NULL;

    if (NULL == THIS) {
        ASSERT(NULL == p_rslt);
        goto FINAL;
    }

    // 更新
    ASSERT(THIS->m_end.mp_node == &THIS->m_head);
    p_rslt = &THIS->m_end.m_iter;

FINAL:
    return p_rslt;
}

void ldlist_destroy(ldlist_t *const THIS)
{
    ldlist_frame_node_t *p_pos = NULL;
    ldlist_frame_node_t *p_cur_next = NULL;

    if (NULL == THIS) {
        goto FINAL;
    }

    LDLIST_FRAME_FOR_EACH_SAFE(p_pos, p_cur_next, &THIS->m_head) {
        // 移除
        ldlist_node_t *p_node = LDLIST_FRAME_ENTRY(p_pos,
                                                   ldlist_node_t,
                                                   m_node);
        ldlist_frame_del(p_pos);

        // 释放
        MEMPOOL_FREE(THIS->mp_mempool, p_node->mp_data);
        p_node->mp_data = NULL;
        MEMPOOL_FREE(THIS->mp_mempool, p_node);
    }

    THIS->mp_mempool = NULL;
    ASSERT(ldlist_frame_node_alone(&THIS->m_head));
    THIS->m_element_size = 0;

FINAL:
    return;
}
