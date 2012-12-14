#include "adv_string.h"
#include "mem.h"
#include "error_info.h"


int_t build_adv_string(adv_string_t *const THIS)
{
#define DEFAULT_STRING_CAPACITY             64

    int_t rslt = 0;
    mempool_t *p_public_pool = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL == THIS->mp_data); // 不允许重复初始化

    THIS->m_size = 0;
    THIS->m_capacity = DEFAULT_STRING_CAPACITY;

    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    THIS->mp_data = MEMPOOL_ALLOC(p_public_pool, THIS->m_capacity);
    if (NULL == THIS->mp_data) {
        rslt = -E_OUT_OF_MEM;
    }

    return rslt;

#undef DEFAULT_STRING_CAPACITY
}


void destroy_adv_string(adv_string_t *const THIS)
{
    mempool_t *p_public_pool = NULL;

    // 参数检测
    ASSERT(NULL != THIS);

    // 定位内存池
    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));

    // 析构
    ASSERT(NULL != THIS->mp_data);
    ASSERT(0 == MEMPOOL_FREE(p_public_pool, THIS->mp_data));
    THIS->mp_data = NULL;
    THIS->m_size = 0;
    THIS->m_capacity = 0;

    return;
}
