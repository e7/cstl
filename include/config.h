// config.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif


// configuration
#define WINDOWS_PLATFORM        0x00000010
#define LINUX_PLATFORM          0x00000020
#define PLATFORM_CFG            LINUX_PLATFORM

#define DEBUG_EDITION           0x00000001

#define MEMPOOL_ISOLATION       0x00000000


#ifdef __cplusplus
} // extern "C"
#endif
#endif // __CONFIG_H__
