#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
     // iOS Simulator
#elif TARGET_OS_IPHONE
    // iOS device
#elif TARGET_OS_MAC
    #define __MAC_OS__
#else
#   error "Unknown Apple platform"
#endif
#endif //end of __APPLE__

#ifdef OS_LITEOS

#ifndef __cplusplus
typedef char					_BOOL;		
#ifndef bool
#define bool					_BOOL
#endif

#include "los_typedef.h"

#ifndef true
#define  true 1 
#endif
#ifndef false
#define false 0
#endif

#else
#define    _BOOL			bool
#endif /* __cplusplus */

#else  //not OS_LITEOS
 #ifndef OS_LINUX_DRIVER
#include <stdbool.h>
typedef bool					_BOOL;
#else
#include <linux/types.h>
#ifndef _BOOL
#define _BOOL    bool
#endif
#endif

#endif //OS_LITEOS

#ifndef _VOID
#define  _VOID void  
#endif

typedef unsigned char			_u8;
typedef unsigned short			_u16;
typedef unsigned long long		_u64;

typedef signed char				_s8;
typedef signed short			_s16;
typedef signed long long		_s64;
#if defined WIN32
typedef  char					_CHAR;
#elif defined OS_RTOS
typedef  char					_CHAR;
#elif defined OS_NRF
typedef  char					_CHAR;
#elif defined __MAC_OS__
typedef char                    _CHAR;
#else
typedef  signed char			_CHAR;
#endif

//64bit system compatiblity,handle is 64bit
//#ifdef  __MAC_OS__
typedef void *    GRID_HANDLE;
//#else
//typedef signed int    GRID_HANDLE;
//#endif

typedef signed int				_INT;
typedef signed int              _s32;
typedef unsigned int            _u32;
typedef unsigned int            _UINT;

typedef unsigned char            _UCHAR;
typedef signed short			_SHORT;
typedef unsigned short			_USHORT;
typedef float					_FLOAT;
typedef double					_DOUBLE;
typedef unsigned long			_ULONG;
typedef unsigned char			BYTE;
typedef void*					_GHandle;

typedef  unsigned char			_BYTE;	//用于表示二进制数据

#ifdef IAR_VER_7803
typedef _ULONG                          _uArg;
typedef signed long                     _iArg;
#else
typedef _u32                            _uArg;
typedef _s32                            _iArg;
#endif

//MacOS NULL definition will have warning
#ifndef __MAC_OS__
	#ifndef NULL
		#ifdef __cplusplus
		#define NULL    0
		#else  /* __cplusplus */
		#define NULL    ((void *)0)
		#endif  /* __cplusplus */
	#endif  /* NULL */
#endif




#define _PI						3.1415926535897932384626433832795
#define _E9						1000000000		//1 * 10^9
#define _E6						1000000			//1 * 10^6
#define _E3						1000			//1 * 10^3

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#ifdef OS_NRF
#include "sdk_common.h"
#endif


#endif	//_TYPE_DEF_H_

