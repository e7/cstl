// mem.c
// author: e7, ryuuzaki.uchiha@gmail.com


#include "mem.h"
#include "list.h"


typedef void object_t;
typedef void page_t;
static int const OBJS_PER_PAGE = 64;

static page_t *alloc_page(int const OBJ_SIZE)
{
    void *p_page = NULL;
    int element_size = OBJ_SIZE + sizeof(object_t *);
    byte_t *p_tmp = NULL;

    p_page = calloc(OBJS_PER_PAGE, element_size);
    ASSERT(NULL != p_page);

    // 串连对象
    p_tmp = (byte_t *)p_page;
    for (int i = 0; i < (OBJS_PER_PAGE - 1); ++i) {
        p_tmp += OBJ_SIZE;

        *(object_t **)p_tmp = p_tmp + sizeof(object_t *);
    }

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
