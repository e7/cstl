// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"
#include "list.h"


// ******************** 对象页 ********************
typedef void object_t; // 对象类型
typedef struct {
    object_t *mp_next;
    char m_obj[0];
} obj_shell_t; // 对象壳类型
typedef void page_t; // 页类型

typedef struct {
    obj_shell_t *mp_free_obj_shs; // 空闲对象壳链
} page_header_t; // 页头

static int const OBJS_PER_PAGE = 64;

static page_t *alloc_page(int const OBJ_SIZE)
{
    int const OBJ_SHELL_SIZE = sizeof(obj_shell_t) + OBJ_SIZE;
    int const PAGE_SIZE = sizeof(page_header_t)
                              + OBJ_SHELL_SIZE * OBJS_PER_PAGE;
    void *p_page = NULL;
    page_header_t *p_header = NULL;
    obj_shell_t *p_next_obj_sh = NULL;

    // 分配页
    p_page = malloc(PAGE_SIZE);
    if (NULL != p_page)
    {
        goto FINAL;
    }

    // 页初始化
    memset(p_page, 0, PAGE_SIZE);
    p_header = (page_header_t *)p_page;
    p_header->mp_free_obj_shs = (obj_shell_t *)(p_header + 1); // 第一个对象壳
    p_next_obj_sh = p_header->mp_free_obj_shs;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_next_obj_sh->mp_next
            = (object_t *)((byte_t *)&p_next_obj_sh->mp_next + OBJ_SIZE);
        ++p_next_obj_sh;
    }

FINAL:
    return p_page;
}

static void free_page(page_t *p_page)
{
    free(p_page);

    return NONE;
}


typedef struct {
    void *mp_page_partial;
    void *mp_page_full;
} std_page_cache;

typedef struct {
    void *mp_page;
    void *mp_object_free;
} std_obj_cache;

static struct {
    int const M_OBJ_SIZE;
    std_obj_cache m_obj_cache;
    std_page_cache m_page_cache;
} sa_mem_cache[] = {
    {
        1 << 3,
    },
    {
        1 << 4,
    },
    {
        1 << 5,
    },
    {
        1 << 6,
    },
    {
        (1 << 6) + (1 << 5),
    },
    {
        1 << 7,
    },
    {
        (1 << 7) + (1 << 6),
    },
    {
        1 << 8,
    },
    {
        1 << 9,
    },
    {
        1 << 10,
    },
    {
        1 << 11,
    },
};

int mempool_init(void)
{
    int rslt = 0;
    int obj_size = 0;

    for (int i = 0; i < ARRAY_COUNT(sa_mem_cache); ++i) {
        obj_size = sa_mem_cache[i].M_OBJ_SIZE;

        sa_mem_cache[i].m_obj_cache.mp_page = alloc_page(obj_size);
    }

    return rslt;
}

void mempool_exit(void)
{
    for (int i = 0; i < ARRAY_COUNT(sa_mem_cache); ++i) {
    }
}
