#include "SysPrint.h"
#include "SysConfig.h"
#include "CodeDef.h"
#include "EncryKey.h"
ENCRY_KEY_1()

#ifdef OS_RTOS
#define MAX_PRINT_CHAR	128
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#elif  defined OS_NRF
#include <stdio.h>
#include <stdarg.h>
#define MAX_PRINT_CHAR	128
#elif  defined OS_LINUX_DRIVER
#define MAX_PRINT_CHAR	128
#include <linux/kernel.h>

#elif defined OS_LITEOS
#include <stdio.h>
#include <stdarg.h>
#define MAX_PRINT_CHAR	2560
#else
#include <stdio.h>
#include <stdarg.h>
#define MAX_PRINT_CHAR	1280
#endif

static PrintCB g_printf = NULL;

_VOID setSysPrintCB(PrintCB p_function){
	g_printf = p_function;
}

//modify by xxh at 2017-12-01 ºı–°BSS”√¡ø
static _CHAR *szMessage = NULL; 

_CHAR *getMessageBuffer(_VOID)
{
	if (szMessage == NULL){
		szMessage = SYS_MALLOC(MAX_PRINT_CHAR);
		DBG_ASSERT(szMessage != NULL);
		//printf("mallocPrintBuf:%d\r\n", MAX_PRINT_CHAR);
	}
	
	memset(szMessage, 0, MAX_PRINT_CHAR);
	return szMessage;
}

_VOID SysPrint_uninit(_VOID)
{
	if (szMessage != NULL){
		SYS_FREE(szMessage);
	}
}

_INT sys_print(const _CHAR* p_szMsg, const _u32 p_nLen)
{
	_INT nRet = 0;
#ifdef OS_RTOS
	if (NULL != g_printf){
		g_printf(p_szMsg, p_nLen);
	}
#elif defined OS_NRF
	printf("%s", p_szMsg);
#elif defined OS_LINUX_DRIVER
	//printk(KERN_CRIT "%s", p_szMsg);
	printk("%s", p_szMsg);
#elif defined OS_LITEOS
	if (NULL != g_printf){
		g_printf(p_szMsg, p_nLen);
	}
	else{
		printf("%s", p_szMsg);
	}
#else
	if (NULL != g_printf){
		g_printf(p_szMsg, p_nLen);
	}
	else{
		nRet = printf("%s", p_szMsg);
	}
#endif
	return nRet;
}


#ifdef OS_RTOS

#else
_INT sys_print_g(const _CHAR* p_szFormat, ...)
{
	_CHAR *buf = getMessageBuffer();
	va_list args;
	va_start(args, p_szFormat);
	V_SPRINTF(buf, p_szFormat, args);
	va_end(args);

	_INT nLen = (_INT)strlen(buf);
	return sys_print(buf, nLen);
}
#endif

_VOID sys_printTime(_VOID)
{
	_u64 u64CurTime_ms = u64_DIV(getUpTime_us(), 1000); 
	_u32 u32Time_s = (_u32)(u64_DIV(u64CurTime_ms, 1000));
	_u32 u32Time_ms = (_u32)(REM_64_DIV(u64CurTime_ms, 1000));	
	sys_print_g("%u.%u%u%u\t", u32Time_s, u32Time_ms / 100, (u32Time_ms % 100) / 10, u32Time_ms % 10);
}

_VOID sys_printData(const _CHAR* p_szMsg, const _CHAR* p_szData, const _u32 p_u32DataLen)
{
	sys_printTime();
	sys_print_g("%s %u:", p_szMsg, p_u32DataLen);
	for (size_t i = 0; i < p_u32DataLen;i++)
	{
		sys_print_g("%x ", (_u8)p_szData[i]);
	}
	sys_print_g("%s","\r\n");
}
