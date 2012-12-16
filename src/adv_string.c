#include "adv_string.h"
#include "mem.h"
#include "error_info.h"


// ***** kmp串查找 *****
static int_t find_max_repeat(char const *pc_str, int_t pass_len);
static int_t find_string_kmp(char const *pc_str, char const *pc_sub);

static int_t adv_string_reserve(adv_string_t *const THIS, size_t capacity);


// ***** 高级字符串接口实现 *****
int_t adv_string_build(adv_string_t *const THIS)
{
    #define DEFAULT_STRING_CAPACITY             4

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


int_t adv_string_reserve(adv_string_t *const THIS, size_t const CAPACITY)
{
    int_t rslt = 0;
    char *p_tmp = NULL;
    mempool_t *p_public_pool = NULL;

    ASSERT(NULL != THIS);
    ASSERT(CAPACITY > THIS->m_capacity);

    // 分配内存
    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    p_tmp = (char *)MEMPOOL_ALLOC(p_public_pool, CAPACITY);
    if (NULL == p_tmp) {
        rslt = -E_OUT_OF_MEM;
        goto FINAL;
    }

    // 拷贝
    ASSERT(NULL != THIS->mp_data);
    (void)memmove(p_tmp, THIS->mp_data, THIS->m_size);
    p_tmp[THIS->m_size] = '\0';

    // 释放原内存并接管新内存
    ASSERT(0 == MEMPOOL_FREE(p_public_pool, THIS->mp_data));
    THIS->mp_data = p_tmp;
    THIS->m_capacity = CAPACITY;

FINAL:
    return rslt;
}


int_t adv_string_append(adv_string_t *const THIS, char const *pc_str)
{
    int_t rslt = 0;
    size_t str_len = 0;
    size_t new_size = 0;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_str);

    str_len = strlen(pc_str);
    new_size = THIS->m_size + str_len;

    ASSERT(NULL != THIS->mp_data);
    if ((THIS->m_capacity - THIS->m_size) < (str_len + 1)) { // 剩余空间不够
        rslt = adv_string_reserve(THIS, 2 * (THIS->m_size + str_len));
        if (0 != rslt) {
            goto FINAL;
        }
    }

    // 追加
    (void)memmove(&THIS->mp_data[THIS->m_size], pc_str, str_len);
    THIS->mp_data[new_size] = '\0';
    THIS->m_size = new_size;

FINAL:
    return rslt;
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
    int_t rslt = 0;
    size_t new_str_len = 0;
    mempool_t *p_public_pool = NULL;

    ASSERT(NULL != THIS);
    ASSERT(NULL != pc_new);

    new_str_len = strlen(pc_new);
    ASSERT(NULL != THIS->mp_data);
    ASSERT(0 == find_mempool(PUBLIC_MEMPOOL, &p_public_pool));
    if (NULL == pc_old) { // 完全替换
        if (THIS->m_capacity < (new_str_len + 1)) {
            ASSERT(0 == MEMPOOL_FREE(p_public_pool, THIS->mp_data));
            THIS->mp_data = MEMPOOL_ALLOC(p_public_pool, 2 * new_str_len);
            if (NULL == THIS->mp_data) {
                rslt = -E_OUT_OF_MEM;

                goto FINAL;
            }
        }
        (void)memcpy(THIS->mp_data, pc_new, new_str_len);
        THIS->mp_data[new_str_len] = '\0';
    } else if (0 == strlen(pc_old)) { // 追加

    } else { // 替换第一次匹配
        rslt = find_string_kmp(THIS->mp_data, pc_old);

        if (rslt < 0) {
            goto FINAL;
        }

    }

FINAL:
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
