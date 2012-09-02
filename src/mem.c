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

    // 检验支持的对象大小
    ASSERT(OBJ_SIZE <= MAX_OBJ_SIZE);

    // 分配页
    p_page = malloc(PAGE_SIZE);
    if (NULL == p_page) {
        goto FINAL;
    }

    // ***** 页初始化 *****
    memset(p_page, 0, PAGE_SIZE);

    init_ldlist_node(&p_page->m_ldlist_node); // 页节点

    p_page->m_use_count = 0; // 对象使用计数

    p_page->mp_free_list = (obj_shell_t *)p_page->ma_obj_shs; // 空闲链
    p_next_obj_sh = p_page->mp_free_list;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_next_obj_sh->m_intptr.mp_next
            = (obj_shell_t *)((byte_t *)p_next_obj_sh + OBJ_SHELL_SIZE);
        p_next_obj_sh = p_next_obj_sh->m_intptr.mp_next;
    }
    p_next_obj_sh->m_intptr.mp_next = NULL;

    p_page->mp_objs_start = (obj_shell_t *)p_page->ma_obj_shs; // 对象组起始

    p_page->mp_objs_end = (obj_shell_t *)(
                              (byte_t *)(p_page->mp_objs_start)
                                  + OBJ_SHELL_SIZE * OBJS_PER_PAGE
                          ); // 对象组终止

FINAL:
    return p_page;
}

static int is_page_full(page_t *p_page)
{
    int rslt = FALSE;

    ASSERT(NULL != p_page);
    if (NULL == p_page->mp_free_list) {
        ASSERT(OBJS_PER_PAGE == p_page->m_use_count);
        rslt = TRUE;
    }
    if (OBJS_PER_PAGE == p_page->m_use_count) {
        ASSERT(NULL == p_page->mp_free_list);
        rslt = TRUE;
    }

    return rslt;
}

static int is_page_empty(page_t *p_page)
{
    ASSERT(NULL != p_page);

    return (0 == p_page->m_use_count);
}

static obj_shell_t *do_provide_obj_sh(page_t *p_page, int const OBJ_SIZE)
{
    obj_shell_t *p_obj_sh = NULL;

    ASSERT(NULL != p_page);
    ASSERT(NULL != p_page->mp_free_list);

    p_obj_sh = p_page->mp_free_list;
    p_page->mp_free_list = p_obj_sh->m_intptr.mp_next;
    p_obj_sh->m_intptr.m_size = OBJ_SIZE;
    ++(p_page->m_use_count);

    return p_obj_sh;
}

static void do_recycle_obj_sh(page_t *p_page, obj_shell_t *p_obj_sh)
{
    ASSERT(NULL != p_page);
    ASSERT(NULL != p_obj_sh);

    p_obj_sh->m_intptr.mp_next = p_page->mp_free_list;
    p_page->mp_free_list = p_obj_sh;
    --(p_page->m_use_count);
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

    p_obj_sh = do_provide_obj_sh(p_page_home, p_base->m_obj_size);

    if (is_page_full(p_page_home)) {
        // 加入到满页链表
        ldlist_add_head(&p_base->m_ldlist_full, &p_page_home->m_ldlist_node);
        p_base->mp_page_current = NULL;
    }

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
            && (p_obj_sh < p_base->mp_page_current->mp_objs_end))
    {
        p_page_home = p_base->mp_page_current;
    } else {
        do {
            int found = FALSE;
            ldlist_node_t *p_node = NULL;

            LDLIST_FOR_EACH(p_node, &p_base->m_ldlist_partial) {
                p_page_home = LDLIST_ENTRY(p_node, page_t, m_ldlist_node);
                if ((p_obj_sh >= p_page_home->mp_objs_start)
                        && (p_obj_sh < p_page_home->mp_objs_end))
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
                        && (p_obj_sh < p_page_home->mp_objs_end))
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
    do_recycle_obj_sh(p_page_home, p_obj_sh);
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
    return mempool_array_alloc(p_mempool, 1, obj_size);
}

void *mempool_array_alloc(mempool_t *p_mempool,
                          int obj_count,
                          int obj_size)
{
    void *p_rslt = NULL;
    int const OBJS_SIZE = obj_size * obj_count;

    if (NULL == p_mempool){
        goto FINAL;
    }

    if (OBJS_SIZE < MIN_OBJ_SIZE) {
        goto FINAL;
    }

    // ********** 分配内存 **********
    if (OBJS_SIZE > MAX_OBJ_SIZE) { // 大对象
        obj_shell_t *p_obj_sh
            = (obj_shell_t *)malloc(sizeof(obj_shell_t) + OBJS_SIZE);
        if (NULL == p_obj_sh) {
            ASSERT(NULL == p_rslt);
            goto FINAL;
        }
        p_obj_sh->m_intptr.m_size = OBJS_SIZE;
        p_rslt = (void *)(p_obj_sh->m_obj);
    } else {
        int index = 0;

        // 寻找合适大小的页仓库
        for (index = 0; index < OBJ_SIZE_NUM; ++index) {
            if (OBJS_SIZE <= A_OBJ_SIZE_SURPPORT[index]) {
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
    obj_shell_t *p_obj_sh = CONTAINER_OF(p_obj, obj_shell_t, m_obj);

    if (NULL == p_mempool) {
        goto FINAL;
    }

    if (NULL == p_obj) {
        goto FINAL;
    }

    ASSERT(NULL != p_obj_sh);
    if (p_obj_sh->m_intptr.m_size > MAX_OBJ_SIZE) { // 大对象，直接释放
        free(p_obj_sh);
    } else {
        int const OBJ_SIZE = p_obj_sh->m_intptr.m_size;
        page_base_t *p_base = NULL;

        for (int i = 0; i < OBJ_SIZE_NUM; ++i) {
            if (OBJ_SIZE == p_mempool->ma_obj_cache[i].m_obj_size) {
                p_base = &p_mempool->ma_obj_cache[i];
            }
        }

        ASSERT(NULL != p_base);
        recycle_obj_sh(p_base, p_obj_sh);
    }   

FINAL:
    return;
}

void mempool_destroy(mempool_t *p_mempool)
{
    ASSERT(NULL != p_mempool);

    for (int i =0; i < OBJ_SIZE_NUM; ++i) {
        ldlist_head_t *p_list = NULL;
        ldlist_node_t *p_pos = NULL;
        ldlist_node_t *p_cur_next = NULL;

        del_page(p_mempool->ma_obj_cache[i].mp_page_current);
        p_mempool->ma_obj_cache[i].mp_page_current = NULL;

        p_list = &p_mempool->ma_obj_cache[i].m_ldlist_partial;
        LDLIST_FOR_EACH_SAFE(p_pos, p_cur_next, p_list) {
            page_t *p_page = LDLIST_ENTRY(p_pos, page_t, m_ldlist_node);
            ldlist_del(p_pos);
            del_page(p_page);
        }

        p_list = &p_mempool->ma_obj_cache[i].m_ldlist_full;
        LDLIST_FOR_EACH_SAFE(p_pos, p_cur_next, p_list) {
            page_t *p_page = LDLIST_ENTRY(p_pos, page_t, m_ldlist_node);
            ldlist_del(p_pos);
            del_page(p_page);
        }
    }
}
