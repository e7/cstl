// adv_string.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __ADV_STRING_H__
#define __ADV_STRING_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


#define DEFAULT_STRING_CAPACITY             32

typedef struct {
    char *mp_data;
    int_t m_size; // 字符串长度
    int_t m_capacity; // 缓冲大小
} adv_string_t;


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __ADV_STRING_H__
