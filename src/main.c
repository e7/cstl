// main.c 各种进程生存期单例对象
// author: e7, ryuuzaki.uchiha@gmail.com

#include "mem.h"
#include "error_info.h"


// ********** 持久内存 **********
static byte_t *sp_permanent_mem = NULL;
static byte_t *sp_pm_current = NULL;
static ssize_t sp_pm_left = 0;

#define PERMANENT_MEM_SIZE          (1024 * 4)
static void build_permanent_mem(ssize_t size)
{
    ASSERT(NULL == sp_permanent_mem);

    sp_permanent_mem = (byte_t *)malloc(size);
    sp_pm_current = sp_permanent_mem;
    sp_pm_left = size;
}

int_t alloc_permanent_mem(ssize_t size, void **pp_mem)
{
    int_t rslt = 0;

    ASSERT(NULL != sp_permanent_mem);

    if ((size < 0) || (size > sp_pm_left)) {
        rslt = -E_OUT_OF_RANGE;

        goto FINAL;
    }
    if (NULL == pp_mem) {
        rslt = -E_NULL_POINTER;

        goto FINAL;
    }

    *pp_mem = sp_pm_current;
    sp_pm_current += size;
    sp_pm_left -= size;

FINAL:
    return rslt;
}

static void destroy_permanent_mem(void)
{
    ASSERT(NULL != sp_permanent_mem);

    free(sp_permanent_mem);

    sp_permanent_mem = NULL;
    sp_pm_current = NULL;
    sp_pm_left = 0;
}


// ********** 库本身的构造析构 **********

static int_t build_cstl(void)
{
    int_t rslt = 0;
    mempool_t *p_mp_public = NULL;

    // 申请持久内存
    build_permanent_mem(PERMANENT_MEM_SIZE);

    // 构造公共内存池
    rslt = alloc_permanent_mem(sizeof(mempool_t), (void *)&p_mp_public);
    if (0 != rslt) {
        goto FINAL;
    }
    ASSERT(NULL != p_mp_public);
    rslt = mempool_build(p_mp_public, PUBLIC_MEMPOOL);

FINAL:
    return rslt;
}

static void destroy_cstl(void)
{
    mempool_t *p_mp_public = NULL;

    // 析构公共内存池
    if (0 == find_mempool(PUBLIC_MEMPOOL, &p_mp_public)) {
        ASSERT(0 == mempool_destroy(p_mp_public));
    } else {
        ASSERT(0);
    }

    // 释放持久内存
    destroy_permanent_mem();

    return;
}


int_t main(int_t argc, char *argv[])
{
    int_t rslt = 0;

    rslt = build_cstl();
    if (E_OK == rslt) {
        rslt = cstl_main(argc, argv);
    }
    destroy_cstl();

    return rslt;
}
