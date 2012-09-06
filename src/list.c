#include "list.h"


typedef struct std_ldlist_node {
    ldlist_node_t m_node;
    void *mp_data;
} std_ldlist_node_t;


int ldlist_build(ldlist_t *const THIS, mempool_t *p_mempool)
{
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
    init_ldlist_node(&THIS->m_head);
    THIS->m_size = 0;

FINAL:
    return rslt;
}

int ldlist_push_front(ldlist_t *const THIS, void const *pc_data)
{
    int rslt = 0;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_data);

    return rslt;
}

void ldlist_destroy(ldlist_t *const THIS)
{
}
