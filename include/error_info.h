// error_info.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __ERROR_INFO_H__
#define __ERROR_INFO_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    E_OK = 0,
    E_UNKNOWN = 1,
    E_OUT_OF_MEM = 4096,
    E_OUT_OF_RANGE,
    E_EXISTED,
    E_NOT_EXIST,
} errno_t;


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __ERROR_INFO_H__
