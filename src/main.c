// main.c 各种进程生存期单例对象
// author: e7, ryuuzaki.uchiha@gmail.com

#include "mem.h"


#define PERMANENT_MEM_SIZE          (1024 * 4)


// ********** 持久内存 **********
static byte_t *sp_permanent_mem = NULL;
static byte_t *sp_pm_current = NULL;
static int_t sp_pm_left = 0;


// 构建持久内存
// 返回值：成功 - E_OK
//         内存耗尽 - E_OUT_OF_MEM
static int_t build_permanent_mem(int_t size)
{
    int_t rslt = E_OK;

    ASSERT(size > 0);

    ASSERT(NULL == sp_permanent_mem);
    sp_permanent_mem = (byte_t *)malloc(size);
    if (NULL == sp_permanent_mem) {
        rslt = -E_OUT_OF_MEM;

        goto FINAL;
    }
    sp_pm_current = sp_permanent_mem;
    sp_pm_left = size;

FINAL:
    return rslt;
}

// 分配持久内存
// 返回值：成功 - E_OK
//         内存耗尽 - E_OUT_OF_MEM
int_t alloc_permanent_mem(int_t size, void **pp_mem)
{
    int_t rslt = 0;

    // 参数检查
    ASSERT(size > 0);
    ASSERT(NULL != pp_mem);

    ASSERT(NULL != sp_permanent_mem);
    if (size > sp_pm_left) {
        rslt = -E_OUT_OF_MEM;

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
    if (NULL != sp_permanent_mem) {
        free(sp_permanent_mem);
    }

    sp_permanent_mem = NULL;
    sp_pm_current = NULL;
    sp_pm_left = 0;
}


// ********** 库本身的构造析构 **********

static bool_t build_cstl(void)
{
    bool_t rslt = TRUE;
    mempool_t *p_mp_public = NULL;

    // 申请持久内存
    if (0 == build_permanent_mem(PERMANENT_MEM_SIZE)) {
        DO_NOTHING();
    } else if (E_OUT_OF_MEM == rslt) {
        rslt = FALSE;

        // log
        (void)fprintf(stderr, "run out of mem!\n");

        goto FINAL;
    } else {
        ASSERT(0);
    }

    // 构造公共内存池
    if (0 == alloc_permanent_mem(sizeof(mempool_t), (void *)&p_mp_public)) {
        DO_NOTHING();
    } else if (E_OUT_OF_MEM == rslt) {
        rslt = FALSE;

        // log
        (void)fprintf(stderr, "out of mem!\n");

        goto FINAL;
    } else {
        ASSERT(0);
    }
    (void)mempool_build(p_mp_public, PUBLIC_MEMPOOL);

FINAL:
    return rslt;
}

static void destroy_cstl(void)
{
    mempool_t *p_mp_public = NULL;

    // 析构公共内存池
    if (0 == find_mempool(PUBLIC_MEMPOOL, &p_mp_public)) {
        mempool_destroy(p_mp_public);
    }

    // 释放持久内存
    destroy_permanent_mem();

    return;
}


int_t main(int_t argc, char *argv[])
{
    int_t rslt = 0;

    if (build_cstl()) {
        rslt = cstl_main(argc, argv);
    }
    destroy_cstl();

    return rslt;
}
