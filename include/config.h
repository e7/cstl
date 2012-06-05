// config.h: configuration for compilation.
// author: e7, ryuuzaki.uchiha@gmail.com


#ifndef __CONFIG_H__
#define __CONFIG_H__


// configuration
#define WINDOWS_PLATFORM        0x00000001
#define LINUX_PLATFORM          0x00000002
#define PLATFORM_CFG            LINUX_PLATFORM


// check configuration
#if !defined(PLATFORM_CFG)
    #error macro PLATFORM_CFG undefined!
#endif
#if (PLATFORM_CFG != LINUX_PLATFORM) && (PLATFORM_CFG != WINDOWS_PLATFORM)
    #error unsurpported platform!
#endif
#endif // __CONFIG_H__
