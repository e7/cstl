// list_frame.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#include "list_frame.h"
#include "iterator.h"
#include "mem.h"


// ******************** 循环双链表接口 ********************
// 循环双链表
typedef struct ldlist {
    mempool_t *mp_mempool;
    ldlist_head_t m_head;
    int m_size;
} ldlist_t;

extern int ldlist_build(ldlist_t *const THIS, mempool_t *p_mempool);

extern int ldlist_push_front(ldlist_t *const THIS, void const *pc_data);

extern int ldlist_push_back(ldlist_t *const THIS, void const *pc_data);

extern void *ldlist_pop_front(ldlist_t *const THIS);

extern void *ldlist_pop_back(ldlist_t *const THIS);

extern iterator_t *ldlist_begin(ldlist_t *const THIS);

extern iterator_t *ldlist_end(ldlist_t *const THIS);

extern void ldlist_destroy(ldlist_t *const THIS);
