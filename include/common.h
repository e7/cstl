// common.h
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __STD__COMMON_H__
#define __STD__COMMON_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


// check configuration
#if !defined(PLATFORM_CFG)
    #error macro PLATFORM_CFG undefined!
#endif
#if (PLATFORM_CFG != LINUX_PLATFORM) && (PLATFORM_CFG != WINDOWS_PLATFORM)
    #error unsurpported platform!
#endif
#endif // __STD__COMMON_H__
