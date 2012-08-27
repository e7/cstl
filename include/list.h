// list_frame.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#include "list_frame.h"
#include "iterator.h"


// ******************** 循环双链表接口 ********************
// 循环双链表
typedef struct ldlist {
    ldlist_head_t m_head;
    int m_size;
} std_ldlist_t;

extern void std_init_ldlist(std_ldlist_t *const THIS);

extern int std_ldlist_push_front(std_ldlist_t *const THIS,
                                 void const *pc_data);

extern int std_ldlist_push_back(std_ldlist_t *const THIS,
                                void const *pc_data);

extern void *std_ldlist_pop_front(std_ldlist_t *const THIS);

extern void *std_ldlist_pop_back(std_ldlist_t *const THIS);

extern iterator_t *std_ldlist_begin(std_ldlist_t *const THIS);

extern iterator_t *std_ldlist_end(std_ldlist_t *const THIS);

extern void std_uninit_ldlist(std_ldlist_t *const THIS);
