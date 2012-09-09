// iterator.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD_ITERATOR_H__
#define __STD_ITERATOR_H__


#include "common.h"


// 迭代器接口
typedef struct iterator iterator_t;
struct iterator {
    void *(*mpf_get_data)(iterator_t const *);
    void (*mpf_set_data)(iterator_t const *, void const *);
    int (*mpf_equal)(iterator_t const *, iterator_t const *);
    iterator_t *(*mpf_get_next)(iterator_t const *);
    iterator_t *(*mpf_get_prev)(iterator_t const *);
};
#endif // __STD_ITERATOR_H__
