// adv_string.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __ADV_STRING_H__
#define __ADV_STRING_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int_t m_size; // 字符串长度
    int_t m_capacity; // 缓冲大小
    char *mp_data;
} adv_string_t;
#define ADV_STRING(name)                    \
            adv_string_t name = {\
                0, 0, NULL,\
            }

// 高级字符串接口
extern int_t build_adv_string(adv_string_t *const THIS);
extern void destroy_adv_string(adv_string_t *const THIS);


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __ADV_STRING_H__
