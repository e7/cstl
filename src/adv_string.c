#include "adv_string.h"
#include "mem.h"
#include "error_info.h"


int_t build_adv_string(adv_string_t *const THIS,
                       char const *pc_string)
{
#define DEFAULT_STRING_CAPACITY             64
    int_t rslt = 0;
    mempool_t *p_public_pool = NULL;

    if ((NULL == THIS) || (NULL == pc_string)) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    if (NULL != THIS->mp_data) {
        rslt = destroy_adv_string(THIS);
    }

    THIS->m_size = strlen(pc_string);
    THIS->m_capacity = MAX(DEFAULT_STRING_CAPACITY, strlen(pc_string));

    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    THIS->mp_data = MEMPOOL_ALLOC(p_public_pool, THIS->m_capacity);
    (void)strncpy(THIS->mp_data, pc_string, THIS->m_size);
    if (THIS->m_size > 0) {
        THIS->mp_data[THIS->m_size - 1] = '\0';
    }

FINAL:
    return rslt;
#undef DEFAULT_STRING_CAPACITY
}


int_t destroy_adv_string(adv_string_t *const THIS)
{
    int_t rslt = 0;
    mempool_t *p_public_pool = NULL;

    if (NULL == THIS) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    if (NULL != THIS->mp_data) {
        MEMPOOL_FREE(p_public_pool, THIS->mp_data);
        THIS->mp_data = NULL;
        THIS->m_size = 0;
        THIS->m_capacity = 0;
    }

FINAL:
    return rslt;
}
