#include "adv_string.h"
#include "mem.h"
#include "error_info.h"


// ***** kmp串查找 *****
static int_t find_max_repeat(char const *pc_str, int_t pass_len);
static int_t find_string_kmp(char const *pc_str, char const *pc_sub);


// ***** 高级字符串接口实现 *****
int_t adv_string_build(adv_string_t *const THIS)
{
    #define DEFAULT_STRING_CAPACITY             64

    int_t rslt = 0;
    mempool_t *p_public_pool = NULL;

    ASSERT(NULL != THIS);

    // 初始化
    THIS->m_size = 0;
    THIS->m_capacity = DEFAULT_STRING_CAPACITY;

    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    if (NULL == THIS->mp_data) {
        THIS->mp_data = MEMPOOL_ALLOC(p_public_pool, THIS->m_capacity);
    } else {
        ASSERT(0); // 不允许重复初始化
    }
    if (NULL == THIS->mp_data) {
        rslt = -E_OUT_OF_MEM;
    }

    return rslt;

    #undef DEFAULT_STRING_CAPACITY
}


int_t find_max_repeat(char const *pc_str, int_t pass_len)
{
    int_t max_len = 0; // 最大重复长度

    ASSERT(NULL != pc_str);
    ASSERT((1 < pass_len) && (pass_len < strlen(pc_str)));

    for (max_len = pass_len - 1; max_len > 0; --max_len) {
        #define LEFT            0
        #define RIGHT           1

        for (int_t iter[2] = {max_len - 1, pass_len - 1};
             iter[RIGHT] >= 0;
             --iter[LEFT], --iter[RIGHT])
        {
            if (pc_str[iter[LEFT]] != pc_str[iter[RIGHT]]) {
                break;
            }
        }

        #undef RIGHT
        #undef LEFT
    }

    return max_len;
}

int_t find_string_kmp(char const *pc_str, char const *pc_sub)
{
    int_t rslt = -E_NOT_EXIST;

    ASSERT(NULL != pc_str);
    ASSERT(NULL != pc_sub);

    do {
        int_t i = 0;
        int_t j = 0;
        int_t const STR_LEN = strlen(pc_str);
        int_t const SUB_LEN = strlen(pc_sub);

        if (SUB_LEN <= 0) {
            break;
        }

        while((STR_LEN - i) > (SUB_LEN - j)) {
            if (SUB_LEN == j) {
                rslt = i - j;
                break;
            }

            if (pc_str[i] == pc_sub[j]) {
                ++i;
                ++j;
                continue;
            }

            if (j > 1) {
                j = find_max_repeat(pc_sub, j);
            } else {
                --j;
            }

            ++i;
        }
    } while (0);

    return rslt;
}

int_t adv_string_replace(adv_string_t *const THIS,
                         char const *pc_old,
                         char const *pc_new)
{
    int rslt = 0;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_new);

    if (NULL == pc_old) {
    } else if (0 == strlen(pc_old)) {
    } else {
        ASSERT(NULL != THIS->mp_data);

        find_string_kmp(THIS->mp_data, pc_old);
    }

    return rslt;
}


void adv_string_destroy(adv_string_t *const THIS)
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
