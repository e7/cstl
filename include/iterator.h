// iterator.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD_ITERATOR_H__
#define __STD_ITERATOR_H__


#include "common.h"


// 迭代器接口
typedef void iterator_t;
typedef struct i_iterator iterator_it;
struct i_iterator {
    void *(*mpf_get_data)(iterator_t const *);
    void (*mpf_set_data)(iterator_t const *, void *);
    iterator_t *(*mpf_get_next)(iterator_t *);
    iterator_t *(*mpf_get_prev)(iterator_t *);
};
#endif // __STD_ITERATOR_H__
