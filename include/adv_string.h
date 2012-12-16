// adv_string.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __ADV_STRING_H__
#define __ADV_STRING_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    size_t m_size; // 字符串长度
    size_t m_capacity; // 缓冲大小
    char *mp_data;
} adv_string_t;
#define ADV_STRING(name)                    \
            adv_string_t name = {\
                0, 0, NULL,\
            }

// ***** 高级字符串接口 *****
extern int_t adv_string_build(adv_string_t *const THIS);
extern int_t adv_string_append(adv_string_t *const THIS, char const *pc_str);
extern int_t adv_string_replace(adv_string_t *const THIS,
                                char const *pc_old,
                                char const *pc_new);
static inline
char const *adv_string_cstr(adv_string_t *const THIS)
{
    ASSERT(NULL != THIS);

    return THIS->mp_data;
}
extern void adv_string_destroy(adv_string_t *const THIS);


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __ADV_STRING_H__
