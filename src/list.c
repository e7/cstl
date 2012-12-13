#include "list.h"


// ******************** 迭代器实现 ********************
static void *ldlist_iterator_get_data(iterator_t const *pc_iter)
{
    void *p_rslt = NULL;
    ldlist_iterator_t const *pc_ldlist_iter = NULL;
    ldlist_node_t const *pc_ldlist_node = NULL;

    ASSERT(NULL != pc_iter);

    pc_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    pc_ldlist_node
        = CONTAINER_OF(pc_ldlist_iter->mp_node, ldlist_node_t, m_node);
    p_rslt = pc_ldlist_node->mp_data;

    return p_rslt;
}

static void ldlist_iterator_set_data(iterator_t const *pc_iter,
                                     void const *pc_data)
{
    ldlist_iterator_t const *pc_ldlist_iter = NULL;
    ldlist_node_t const *pc_ldlist_node = NULL;

    ASSERT(NULL != pc_iter);
    ASSERT(NULL != pc_data);

    pc_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    pc_ldlist_node
        = CONTAINER_OF(pc_ldlist_iter->mp_node, ldlist_node_t, m_node);
    (void)memcpy(pc_ldlist_node->mp_data,
                 pc_data,
                 pc_ldlist_iter->m_element_size);

    return;
}

static int_t ldlist_iterator_equal(iterator_t const *pc_iter1,
                                   iterator_t const *pc_iter2)
{
    int_t rslt = FALSE;
    ldlist_iterator_t const *pc_ldlist_iter1 = NULL;
    ldlist_iterator_t const *pc_ldlist_iter2 = NULL;

    ASSERT(NULL != pc_iter1);
    ASSERT(NULL != pc_iter2);

    pc_ldlist_iter1 = CONTAINER_OF(pc_iter1, ldlist_iterator_t, m_iter);
    pc_ldlist_iter2 = CONTAINER_OF(pc_iter2, ldlist_iterator_t, m_iter);

    rslt = (pc_ldlist_iter1->mp_node == pc_ldlist_iter2->mp_node)
               ? TRUE : FALSE;

    return rslt;
}

static iterator_t *ldlist_iterator_get_next(iterator_t const *pc_iter)
{
    iterator_t *p_rslt = NULL;
    ldlist_iterator_t *p_ldlist_iter = NULL;

    ASSERT(NULL != pc_iter);

    p_ldlist_iter = CONTAINER_OF(pc_iter, ldlist_iterator_t, m_iter);
    p_ldlist_iter->mp_node = ldlist_frame_next(p_ldlist_iter->mp_node);
    p_rslt = (iterator_t *)pc_iter;

    return p_rslt;
}

static iterator_t *ldlist_iterator_get_prev(iterator_t const *pc_iter)
{
    iterator_t *p_rslt = NULL;

    ASSERT(NULL != pc_iter);

    return p_rslt;
}


// ******************** 循环双链表接口实现 ********************
void ldlist_build(ldlist_t *const THIS,
                  mempool_t *p_mempool,
                  int_t element_size)
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

    ASSERT(NULL != THIS);
    ASSERT(NULL != p_mempool);

    THIS->mp_mempool = p_mempool;
    init_ldlist_frame_node(&THIS->m_head);
    THIS->m_element_size = element_size;

    memcpy(&THIS->m_begin, &LDLIST_ITER_TPLT, sizeof(LDLIST_ITER_TPLT));
    memcpy(&THIS->m_end, &LDLIST_ITER_TPLT, sizeof(LDLIST_ITER_TPLT));

    return;
}

int_t ldlist_is_empty(ldlist_t *const THIS)
{
    ASSERT(NULL != THIS);

    return ldlist_frame_node_alone(&THIS->m_head);
}

void *ldlist_first(ldlist_t *const THIS)
{
    ldlist_node_t *p_node = NULL;

    ASSERT(NULL != THIS);

    p_node = CONTAINER_OF(THIS->m_head.mp_next, ldlist_node_t, m_node);

    return p_node->mp_data;
}

void *ldlist_last(ldlist_t *const THIS)
{
    ldlist_node_t *p_node = NULL;

    ASSERT(NULL != THIS);

    p_node = CONTAINER_OF(THIS->m_head.mp_prev, ldlist_node_t, m_node);

    return p_node->mp_data;
}

enum {
    PUSH_FRONT = 0x10,
    POP_FRONT  = 0x20,
    PUSH_BACK = 0x11,
    POP_BACK  = 0x21,
};

static void ldlist_push(ldlist_t *const THIS,
                        void const *pc_data,
                        int_t push_type)
{
    ldlist_node_t *p_node = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_data);
    ASSERT(NULL != THIS->mp_mempool);

    /* 构造结点 */
    p_node = MEMPOOL_ALLOC(THIS->mp_mempool, sizeof(ldlist_node_t));
    p_node->mp_data = MEMPOOL_ALLOC(THIS->mp_mempool, THIS->m_element_size);
    (void)memcpy(p_node->mp_data, pc_data, THIS->m_element_size);

    /* 伤一敌可连其百 */
    if (PUSH_FRONT == push_type) {
        ldlist_frame_add_head(&THIS->m_head, &p_node->m_node);
    } else if (PUSH_BACK == push_type) {
        ldlist_frame_add_tail(&THIS->m_head, &p_node->m_node);
    } else {
        ASSERT(0);
    }

    ASSERT(!ldlist_is_empty(THIS));

    return;
}

void ldlist_push_front(ldlist_t *const THIS, void const *pc_data)
{
    /* 条件检查 */
    ASSERT(NULL != THIS);
    ASSERT(NULL != THIS->mp_mempool);
    ASSERT(NULL != pc_data);

    ldlist_push(THIS, pc_data, PUSH_FRONT);

    return;
}

void ldlist_push_back(ldlist_t *const THIS, void const *pc_data)
{
    /* 条件检查 */
    ASSERT(NULL != THIS);
    ASSERT(NULL != THIS->mp_mempool);
    ASSERT(NULL != pc_data);

    ldlist_push(THIS, pc_data, PUSH_BACK);

    return;
}

static void ldlist_pop(ldlist_t *const THIS, int_t pop_type)
{
    ldlist_node_t *p_node = NULL;
    ldlist_frame_node_t *p_frame_node = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL != THIS->mp_mempool);

    if (POP_FRONT == pop_type) {
        p_frame_node = ldlist_frame_first(&THIS->m_head);
    } else if (POP_BACK == pop_type) {
        p_frame_node = ldlist_frame_last(&THIS->m_head);
    } else {
        ASSERT(0);
    }
    ldlist_frame_del(p_frame_node);
    p_node = CONTAINER_OF(p_frame_node, ldlist_node_t, m_node);
    MEMPOOL_FREE(THIS->mp_mempool, p_node->mp_data);
    MEMPOOL_FREE(THIS->mp_mempool, p_node);
}

void ldlist_pop_front(ldlist_t *const THIS)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != THIS->mp_mempool);

    ldlist_pop(THIS, POP_FRONT);

    return;
}

void ldlist_pop_back(ldlist_t *const THIS)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != THIS->mp_mempool);

    ldlist_pop(THIS, POP_BACK);

    return;
}

iterator_t *ldlist_begin(ldlist_t *const THIS,
                         ldlist_iterator_t *p_iter)
{
    ASSERT(NULL != THIS);
    ASSERT(NULL != p_iter);

    // 更新
    THIS->m_begin.mp_node = ldlist_frame_first(&THIS->m_head);

    (void)memcpy(p_iter, &THIS->m_begin, sizeof(THIS->m_begin));

    return &p_iter->m_iter;
}

iterator_t const *ldlist_end(ldlist_t *const THIS)
{
    iterator_t *p_rslt = NULL;

    ASSERT(NULL != THIS);

    ASSERT(THIS->m_end.mp_node == &THIS->m_head);
    p_rslt = &THIS->m_end.m_iter;

    return p_rslt;
}

void ldlist_clean(ldlist_t *const THIS)
{
    ldlist_frame_node_t *p_pos = NULL;
    ldlist_frame_node_t *p_cur_next = NULL;

    ASSERT(NULL != THIS);

    if (ldlist_frame_node_alone(&THIS->m_head)) {
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
    ASSERT(ldlist_frame_node_alone(&THIS->m_head));

FINAL:
    return;
}

void ldlist_destroy(ldlist_t *const THIS)
{
    ldlist_clean(THIS);

    (void)memcpy(&THIS->m_begin, &THIS->m_end, sizeof(THIS->m_end));
    THIS->m_element_size = 0;
    ASSERT(ldlist_frame_node_alone(&THIS->m_head));
    THIS->mp_mempool = NULL;
}
