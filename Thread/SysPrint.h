#ifndef _SYS_PRINT_H_
#define _SYS_PRINT_H_

#include "TypeDef.h"
#include "SysConfig.h"
#include "SysTime.h"



#ifdef OS_RTOS
#include <ti/drivers/UART.h>
#include <xdc/runtime/System.h>
#include <stdio.h>
#include <string.h>
//#define SPRINTF   System_snprintf
#elif defined OS_LINUX_DRIVER
#include <linux/io.h>
#include <linux/string.h>
#include <linux/kernel.h>
#elif defined OS_LITEOS
#include <stdio.h>
#include <string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#define SPRINTF   sprintf

// 定义打印回调方法,可替换原始打印方法
typedef _INT(*PrintCB)(const _CHAR *p_pszOutBuf, const _INT p_u16BufLen);
_VOID setSysPrintCB(PrintCB p_function);

_CHAR *getMessageBuffer(_VOID);

#ifdef OS_RTOS
#include "GSGString.h"
#define V_SPRINTF(buf,format,...) GSG_sprintf(buf,format,##__VA_ARGS__) 
#define sys_print_g(p_szFormat,...) do \
{\
	_CHAR *buf = getMessageBuffer(); \
	V_SPRINTF(buf, p_szFormat, ##__VA_ARGS__); \
	_INT nLen = (_INT)strlen(buf); \
	sys_print(buf, nLen); \
} while (false)
#else
#define V_SPRINTF(buf,format,...) vsprintf(buf,format,##__VA_ARGS__)
_INT sys_print_g(const _CHAR* p_szFormat, ...);
#endif



_VOID sys_printTime(_VOID);

//打印   不超过1024个字节的字符串
_INT sys_print(const _CHAR* p_szMsg, const _u32 p_nLen);

#ifdef PRINT_ON
#if defined WIN32
	#define _B_PRINT_LN(fmt, ...)  do{\
		_CHAR *szMessage = getMessageBuffer();\
		sprintf(szMessage, fmt, ##__VA_ARGS__); \
		_u32 u16BufLen = (_u32)strlen(szMessage);\
		szMessage[u16BufLen] = '\r';\
		szMessage[u16BufLen+1] = '\n';\
		sys_print(szMessage, u16BufLen + 2); \
	}while (false)

	#define _B_PRINT_(fmt, ...)  do{\
		_CHAR *szMessage = getMessageBuffer(); \
		sprintf(szMessage, fmt, ##__VA_ARGS__); \
		_u32 u16BufLen = (_u32)strlen(szMessage); \
		sys_print(szMessage, u16BufLen); \
	}while (false)
#define _B_PRINT_TIME() sys_printTime()
#else
#ifdef OS_RTOS
//自动在尾部添加换行符
	#define _B_PRINT_(fmt, ...) sys_print_g(fmt, ##__VA_ARGS__)
	#define _B_PRINT_LN(fmt, ...)  sys_print_g(fmt, ##__VA_ARGS__);_B_PRINT_("\r\n")
	#define _B_PRINT_TIME() sys_printTime()
#elif defined OS_NRF
//自动在尾部添加换行符
	#define _B_PRINT_(fmt, ...) sys_print_g(fmt, ##__VA_ARGS__)
	#define _B_PRINT_LN(fmt, ...)  sys_print_g(fmt, ##__VA_ARGS__);_B_PRINT_("\r\n")
	#define _B_PRINT_TIME() sys_printTime()
#else
	#define _B_PRINT_LN(fmt, ...)  do{\
		_CHAR *szMessage = getMessageBuffer();\
		sprintf(szMessage, fmt, ##__VA_ARGS__); \
		_u32 u16BufLen = (_u32)strlen(szMessage);\
		szMessage[u16BufLen] = '\r';\
		szMessage[u16BufLen+1] = '\n';\
		sys_print(szMessage, u16BufLen + 2); \
	}while (false)

	#define _B_PRINT_(fmt, ...)  do{\
		_CHAR *szMessage = getMessageBuffer(); \
		sprintf(szMessage, fmt, ##__VA_ARGS__); \
		_u32 u16BufLen = (_u32)strlen(szMessage); \
		sys_print(szMessage, u16BufLen); \
	}while (false)

	#define _B_PRINT_TIME() sys_printTime()
#endif
#endif
#else
#define _B_PRINT_LN(fmt, ...)
#define _B_PRINT_(fmt, ...)
#define _B_PRINT_TIME() 
#endif

#define _PRINT_(fmt, ...) _B_PRINT_(fmt, ##__VA_ARGS__)
#define _PRINT_LN(fmt, ...) _B_PRINT_LN(fmt, ##__VA_ARGS__)
#define _PRINT_LN_T(fmt, ...) _B_PRINT_TIME(); _B_PRINT_LN(fmt, ##__VA_ARGS__)

_VOID sys_printData(const _CHAR* p_szMsg, const _CHAR* p_szData, const _u32 p_u32DataLen);

#ifdef PRINT_ON
#define _PRINT_DATA(msg,data,len)	sys_printData(msg,(_CHAR *)data,len)
#else
#define _PRINT_DATA(msg,data,len)	
#endif

_VOID SysPrint_uninit(_VOID);

#endif	//_SYS_PRINT_H_
