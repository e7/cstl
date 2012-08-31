// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"


// ******************** 对象页 ********************
static void *provide_obj(page_t *p_page)
{
    void *rslt = NULL;

    ASSERT(NULL != p_page);

    return rslt;
}

static void recycle_obj(page_t *p_page, void const *pc_obj)
{
    ASSERT(NULL != p_page);
    ASSERT(NULL != pc_obj);
}

static page_t *new_page(int const OBJ_SIZE)
{
    int const OBJ_SHELL_SIZE = sizeof(obj_shell_t) + OBJ_SIZE;
    int const PAGE_SIZE = sizeof(page_t) + OBJ_SHELL_SIZE * OBJS_PER_PAGE;
    page_t *p_page = NULL;
    obj_shell_t *p_next_obj_sh = NULL;

    // 检验对象大小
    ASSERT(OBJ_SIZE > MAX_OBJ_SIZE);

    // 分配页
    p_page = malloc(PAGE_SIZE);
    if (NULL == p_page) {
        goto FINAL;
    }

    // ***** 页初始化 *****
    memset(p_page, 0, PAGE_SIZE);

    p_page->m_obj_size = OBJ_SIZE; // 对象大小
    init_ldlist_node(&p_page->m_ldlist_node); // 页节点
    p_page->m_use_count = 0; // 对象使用计数
    p_page->mp_free_obj_shs = (obj_shell_t *)(p_page + 1); // 空闲链
    p_next_obj_sh = p_page->mp_free_obj_shs;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_next_obj_sh->mp_next
            = (obj_shell_t *)((byte_t *)p_next_obj_sh + OBJ_SHELL_SIZE);
        p_next_obj_sh = p_next_obj_sh->mp_next;
    }
    p_page->mp_objs_start = (obj_shell_t *)(p_page + 1); // 对象组起始
    p_page->mp_objs_end = p_page->mp_objs_start + OBJS_PER_PAGE; // 对象组终止

FINAL:
    return p_page;
}

static int is_page_full(page_t *p_page)
{
    ASSERT(NULL != p_page);

    return (NULL == p_page->mp_free_obj_shs);
}

static int is_page_empty(page_t *p_page)
{
    ASSERT(NULL != p_page);

    return (0 == p_page->m_use_count);
}

static void del_page(page_t *p_page)
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
        init_ldlist_node(&p_mempool->ma_obj_cache[i].m_ldlist_partial);
        init_ldlist_node(&p_mempool->ma_obj_cache[i].m_ldlist_full);
        p_mempool->ma_obj_cache[i].mp_page_current= NULL;
    }

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
    int total_size = obj_size * obj_count;

    if (NULL == p_mempool){
        goto FINAL;
    }

    if (obj_size < MIN_OBJ_SIZE) {
        goto FINAL;
    }

    if (total_size > MAX_OBJ_SIZE) { // 大对象
        big_obj_t *p_big_obj = 0;

        p_big_obj = malloc(sizeof(big_obj_t) + obj_size);
        if (NULL == p_big_obj) {
            ASSERT(NULL == p_rslt);
            goto FINAL;
        }
        p_big_obj->m_obj_size = obj_size;
    } else {
        int index = 0;
        int mem_size = 0;
        page_t *p_page_current = NULL;

        // 对象大小取整
        for (index = 0; index < OBJ_SIZE_NUM; ++index) {
            if (total_size < A_OBJ_SIZE_SURPPORT[index]) {
                mem_size = A_OBJ_SIZE_SURPPORT[index];
            } else {
                break;
            }
        }
        ASSERT(index <= OBJ_SIZE_NUM);

        // 分配对象页
        if (NULL == p_mempool->ma_obj_cache[index].mp_page_current) {
            // 部分页链肯定是空的
            ASSERT(ldlist_node_alone(
                       &p_mempool->ma_obj_cache[index].m_ldlist_partial));
            p_mempool->ma_obj_cache[index].mp_page_current
                = new_page(mem_size);
        }
        if (NULL == p_mempool->ma_obj_cache[index].mp_page_current) {
            goto FINAL;
        }

        // 从对象页中分配对象
        ASSERT(NULL != p_mempool->ma_obj_cache[index].mp_page_current);
        p_page_current = p_mempool->ma_obj_cache[index].mp_page_current;
        p_rslt = provide_obj(p_page_current);
        if (is_page_full(p_page_current)) { // 页已满

            // 加入满页链表
            ldlist_add_head(&p_mempool->ma_obj_cache[index].m_ldlist_full,
                            &p_page_current->m_ldlist_node);

            // 从部分页调入
            p_mempool->ma_obj_cache[index].mp_page_current
                = ldlist_del_head(
                      &p_mempool->ma_obj_cache[index].m_ldlist_partial);
        }
    }

FINAL:
    return p_rslt;
}

void mempool_free(mempool_t *p_mempool, void *p_obj)
{
    if (NULL == p_mempool) {
        goto FINAL;
    }

    if (NULL == p_obj) {
        goto FINAL;
    }

    if (*(int *)p_obj > MAX_OBJ_SIZE) { // 大对象，直接释放
        free(p_obj);
    } else {
    }

FINAL:
    return;
}

void mempool_destroy(mempool_t *p_mempool)
{
}
