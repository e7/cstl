// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"


// ******************** 对象页 ********************
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

    init_ldlist_node(&p_page->m_ldlist_node); // 页节点

    p_page->m_use_count = 0; // 对象使用计数

    p_page->mp_free_list = (obj_shell_t *)(p_page + 1); // 空闲链
    p_next_obj_sh = p_page->mp_free_list;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_next_obj_sh->m_intptr.mp_next
            = (obj_shell_t *)((byte_t *)p_next_obj_sh + OBJ_SHELL_SIZE);
        p_next_obj_sh = p_next_obj_sh->m_intptr.mp_next;
    }
    p_next_obj_sh->m_intptr.mp_next = NULL;

    p_page->mp_objs_start = (obj_shell_t *)(p_page + 1); // 对象组起始

    p_page->mp_objs_end = p_page->mp_objs_start + OBJS_PER_PAGE; // 对象组终止

FINAL:
    return p_page;
}

static int is_page_full(page_t *p_page)
{
    ASSERT(NULL != p_page);

    return (NULL == p_page->mp_free_list);
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


// ******************** 页仓库 ********************
static obj_shell_t *provide_obj_sh(page_base_t *p_base)
{
    obj_shell_t *p_obj_sh = NULL;
    page_t *p_page_home = NULL;

    ASSERT(NULL != p_base);

    if (is_page_full(p_page_home)) {
        // 加入到满页链表
        ldlist_add_head(&p_base->m_ldlist_full, &p_page_home->m_ldlist_node);
        p_base->mp_page_current = NULL;
    }
    if (NULL == p_base->mp_page_current) {
        p_base->mp_page_current
            = (page_t *)ldlist_del_head(&p_base->m_ldlist_partial);
    }
    if (NULL == p_base->mp_page_current) {
        p_base->mp_page_current = new_page(p_base->m_obj_size);
    }
    if (NULL == p_base->mp_page_current) {
        goto FINAL;
    }

    ASSERT(NULL != p_base->mp_page_current);
    p_page_home = p_base->mp_page_current;
    ASSERT(NULL != p_page_home->mp_free_list);
    p_obj_sh = p_page_home->mp_free_list;
    p_page_home->mp_free_list = p_obj_sh->m_intptr.mp_next;

    p_obj_sh->m_intptr.m_size = p_base->m_obj_size; // 标志size，记得回家
    ++(p_page_home->m_use_count);

FINAL:
    return p_obj_sh;
}

static void recycle_obj_sh(page_base_t *p_base, obj_shell_t *p_obj_sh)
{
    page_t *p_page_home = NULL;

    ASSERT(NULL != p_base);
    ASSERT(NULL != p_obj_sh);

    // 寻页
    if ((p_obj_sh >= p_base->mp_page_current->mp_objs_start)
            && (p_obj_sh <= p_base->mp_page_current->mp_objs_end))
    {
        p_page_home = p_base->mp_page_current;
    } else {
        do {
            int found = FALSE;
            ldlist_node_t *p_node = NULL;

            LDLIST_FOR_EACH(p_node, &p_base->m_ldlist_partial) {
                p_page_home = LDLIST_ENTRY(p_node, page_t, m_ldlist_node);
                if ((p_obj_sh >= p_page_home->mp_objs_start)
                        && (p_obj_sh <= p_page_home->mp_objs_end))
                {
                    found = TRUE;
                    break;
                }
            }
            if (found) {
                break;
            }
            LDLIST_FOR_EACH(p_node, &p_base->m_ldlist_partial) {
                p_page_home = LDLIST_ENTRY(p_node, page_t, m_ldlist_node);
                if ((p_obj_sh >= p_page_home->mp_objs_start)
                        && (p_obj_sh <= p_page_home->mp_objs_end))
                {
                    break;
                }
            }
        } while (0);
    }

    // 回收对象
    ASSERT(NULL != p_page_home);
    if (p_page_home != p_base->mp_page_current) {
        ldlist_del(&p_page_home->m_ldlist_node);
        ldlist_add_tail(&p_base->m_ldlist_partial,
                        &p_page_home->m_ldlist_node);
    }
    p_obj_sh->m_intptr.mp_next = p_page_home->mp_free_list;
    p_page_home->mp_free_list = p_obj_sh;
    --(p_page_home->m_use_count);
    if (is_page_empty(p_page_home)
            && (p_page_home != p_base->mp_page_current))
    {
        ldlist_del(&p_page_home->m_ldlist_node);
        del_page(p_page_home);
    }
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
        p_mempool->ma_obj_cache[i].m_obj_size = A_OBJ_SIZE_SURPPORT[i];
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
    int const TOTAL_SIZE = obj_size * obj_count;

    if (NULL == p_mempool){
        goto FINAL;
    }

    if (obj_size < MIN_OBJ_SIZE) {
        goto FINAL;
    }

    // ********** 分配内存 **********
    if (TOTAL_SIZE > MAX_OBJ_SIZE) { // 大对象
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

        // 对象大小取整
        for (index = 0; index < OBJ_SIZE_NUM; ++index) {
            if (TOTAL_SIZE < A_OBJ_SIZE_SURPPORT[index]) {
                mem_size = A_OBJ_SIZE_SURPPORT[index];
            } else {
                break;
            }
        }
        ASSERT(index <= OBJ_SIZE_NUM);

        // ***** 从页仓库中分配对象 *****
        p_rslt = (void *)(
                     provide_obj_sh(&p_mempool->ma_obj_cache[index])->m_obj
                 );
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
