// hash_base.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __HASH_BASE_H__
#define __HASH_BASE_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif

// 常用哈希函数
static inline
int_t rs_hash(char const *pc_str, int_t str_len)
{
    int_t b = 378551;
    int_t a = 63689;
    int_t hash = 0;

    ASSERT(0 == hash);
    for (int_t i = 0; i < str_len; ++i) {
        hash = hash * a + pc_str[i];
        a = a * b;
    }

    return hash;
}

static inline
int_t js_hash(char const *pc_str, int_t str_len)
{
    int_t hash = 1315423911;

    for (int_t i = 0; i < str_len; ++i) {
        hash ^= ((hash << 5) + pc_str[i] + (hash >> 2));
    }

    return hash;
}

static inline
int_t elf_hash(char const *pc_str, int_t str_len)
{
    int_t hash = 0;
    int_t x = 0;

    for (int_t i = 0; i < str_len; ++i) {
        hash = (hash << 4) + pc_str[i];
        x = hash & 0xF0000000;
        if (0 != x) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }

    return hash;
}

static inline
int_t sdbm_hash(char const *pc_str, int_t str_len)
{
    int_t hash = 0;

    for (int_t i = 0; i < str_len; ++i) {
        hash = pc_str[i] + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

static inline
int_t djb_hash(char const *pc_str, int_t str_len) // 据说最有效的哈希
{
    int_t hash = 5381;

    for (int_t i = 0; i < str_len; ++i) {
        hash = ((hash << 5) + hash) + pc_str[i];
    }

    return hash;
}

static inline
int_t dek_hash(char const *pc_str, int_t str_len)
{
    int_t hash = str_len;

    for (int_t i = 0; i < str_len; ++i) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ pc_str[i];
    }

    return hash;
}

#ifdef __cplusplus
}
#endif
#endif // __HASH_BASE_H__
