#include "list.h"


typedef struct std_ldlist_node {
    ldlist_node_t m_node;
    void *mp_data;
} std_ldlist_node_t;


void std_init_ldlist(std_ldlist_t *const THIS)
{
    ASSERT(NULL != THIS);

    init_ldlist_node(&THIS->m_head);
    THIS->m_size = 0;
}

int std_ldlist_push_front(std_ldlist_t *const THIS,
                          void const *pc_data)
{
    int rslt = 0;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_data);

    return rslt;
}
