// common.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD__COMMON_H__
#define __STD__COMMON_H__


#include "config.h"


// check configuration
#if !defined(PLATFORM_CFG)
    #error macro PLATFORM_CFG undefined!
#endif
#if (PLATFORM_CFG != LINUX_PLATFORM) && (PLATFORM_CFG != WINDOWS_PLATFORM)
    #error unsurpported platform!
#endif

#if !defined(DEBUG_EDITION)
    #error macro DEBUG_EDITION undefined!
#endif


// common headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#if (LINUX_PLATFORM == PLATFORM_CFG)
    #include <unistd.h>
#endif
#if (WINDOWS_PLATFORM == PLATFORM_CFG)
    #include <windows.h>
#endif


// common types
typedef signed char byte_t;
typedef unsigned char ubyte_t;
typedef signed short word_t;
typedef unsigned short uword_t;
typedef signed int int_t;
typedef unsigned int uint_t;
typedef signed long long_t;
typedef unsigned long ulong_t;


// common macros
#if DEBUG_EDITION
    #define ASSERT(cond)        assert(cond)
#else
    #define ASSERT(cond)        ((void)0)
#endif // DEBUG_EDITION

#define OFFSET_OF(s, m)         ((size_t)&(((s *)0)->m ))
#define CONTAINER_OF(ptr, type, member)     \
            ({\
                const typeof(((type *)0)->member) *p_mptr = (ptr);\
                (type *)((char *)p_mptr - OFFSET_OF(type, member));\
            })
#define ARRAY_COUNT(a)          (sizeof(a) / sizeof((a)[0]))

#define HOWMANY(x, y)           (((x) + ((y) - 1)) / (y))
#endif // __STD__COMMON_H__
