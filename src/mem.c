// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"
#include "list.h"


// ******************** 对象页 ********************
#define OBJS_PER_PAGE               64

typedef void object_t; // 对象类型
typedef struct s_obj_shell obj_shell_t;
struct s_obj_shell {
    obj_shell_t *mp_next;
    char m_obj[0];
}; // 对象壳类型


typedef struct s_page_header page_header_t;
struct s_page_header {
    char m_inuse[HOWMANY(OBJS_PER_PAGE, sizeof(char))]; // 对象使用情况位图
    obj_shell_t *mp_free_obj_shs; // 空闲对象壳链
}; // 页头

typedef struct s_page page_t;
struct s_page {
    page_header_t m_header; // 页头
    page_t *mp_next; // 下一页
}; // 对象页


static page_t *alloc_page(int const OBJ_SIZE)
{
    int const OBJ_SHELL_SIZE = sizeof(obj_shell_t) + OBJ_SIZE;
    int const PAGE_SIZE = sizeof(page_t) + OBJ_SHELL_SIZE * OBJS_PER_PAGE;
    page_t *p_page = NULL;
    obj_shell_t *p_next_obj_sh = NULL;

    // 分配页
    p_page = malloc(PAGE_SIZE);
    if (NULL == p_page)
    {
        goto FINAL;
    }

    // 页初始化
    memset(p_page, 0, PAGE_SIZE);
    p_page->m_header.mp_free_obj_shs = (obj_shell_t *)(p_page + 1);
    p_next_obj_sh = p_page->m_header.mp_free_obj_shs;
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


// ******************** 对象池 ********************
typedef struct {
    void *mp_page_partial;
    void *mp_page_full;
} page_cache_t;

typedef struct {
    void *mp_page;
    void *mp_object_free;
} obj_cache_t;

static struct {
    int const M_OBJ_SIZE;
    obj_cache_t m_obj_cache;
    page_cache_t m_page_cache;
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
