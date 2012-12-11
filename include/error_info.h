// error_info.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __ERROR_INFO_H__
#define __ERROR_INFO_H__

#include "common.h"


typedef enum {
    E_OK = 0,
    E_UNKNOWN = 1,
    E_NULL_POINTER = 4096,
    E_OUT_OF_RANGE,
    E_EXISTED,
    E_NOT_EXISTED,
} errno_t;
#endif // __ERROR_INFO_H__
