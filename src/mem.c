// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"


static page_t *alloc_page(int const OBJ_SIZE)
{
    int const OBJ_SHELL_SIZE = sizeof(obj_shell_t) + OBJ_SIZE;
    int const PAGE_SIZE = sizeof(page_t) + OBJ_SHELL_SIZE * OBJS_PER_PAGE;
    page_t *p_page = NULL;
    obj_shell_t *p_next_obj_sh = NULL;

    // 检验对象大小
    if (OBJ_SIZE > MAX_OBJ_SIZE) {
        goto FINAL;
    }

    // 分配页
    p_page = malloc(PAGE_SIZE);
    if (NULL == p_page) {
        goto FINAL;
    }

    // 页初始化
    memset(p_page, 0, PAGE_SIZE);
    p_page->mp_free_obj_shs = (obj_shell_t *)(p_page + 1);
    p_next_obj_sh = p_page->mp_free_obj_shs;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_next_obj_sh->mp_next
            = (obj_shell_t *)((byte_t *)p_next_obj_sh + OBJ_SHELL_SIZE);
        p_next_obj_sh = p_next_obj_sh->mp_next;
    }

FINAL:
    return p_page;
}

static void free_page(page_t *p_page)
{
    free(p_page);
}


// ******************** 内存池接口 ********************
int mempool_build(mempool_t *p_mempool)
{
    int rslt = 0;

    if (NULL == p_mempool) {
        rslt = -1;
        goto FINAL;
    }

    for (int i = 0; i < OBJ_SIZE_NUM; ++i) {
        p_mempool->ma_obj_cache[i].mp_page_partial = NULL;
        p_mempool->ma_obj_cache[i].mp_page_full = NULL;
        p_mempool->ma_obj_cache[i].mp_page_current= NULL;
    }
    init_ldlist_node(&p_mempool->m_big_obj_list);

FINAL:
    return rslt;
}

void *mempool_alloc(mempool_t *p_mempool, int obj_size)
{
    return mempool_array_alloc(p_mempool, obj_size, 1);
}

void *mempool_array_alloc(mempool_t *p_mempool,
                          int obj_size,
                          int obj_count)
{
    void *p_rslt = NULL;

    if ((NULL == p_mempool) || (obj_size < MIN_OBJ_SIZE)) {
        goto FINAL;
    }
    if ((obj_size * obj_count)> MAX_OBJ_SIZE) { // 大对象
        big_obj_t *p_big_obj = 0;

        p_big_obj = malloc(sizeof(big_obj_t) + obj_size);
        if (NULL == p_big_obj) {
            ASSERT(NULL == p_rslt);
            goto FINAL;
        }
        p_big_obj->m_obj_size = obj_size;
    } else {
        // 对象大小取整
    }

FINAL:
    return p_rslt;
}

void mempool_free(mempool_t *p_mempool, void *p_obj)
{
}

void mempool_destroy(mempool_t *p_mempool)
{
}
