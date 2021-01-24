#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#include "TypeDef.h"

typedef struct _SysTime_s
{
	_UINT		year;
	_UINT		month;			
	_UINT		day;
	_UINT		hour;
	_UINT		minute;
	_UINT		second;
	_UINT		millisecond;
	_UINT		dayOfWeek;		//0-6	0是星期日
	_u64		UTCSecond;		//1970到现在的秒数

}SysTime_s;

#define us2ms(t)    (_u32)(t/1000)

#ifdef OS_NRF
#include "nrf_drv_timer.h"
_VOID RTOS_Time_setTimeConfig(nrf_drv_timer_config_t *p_pstTimer_cfg);
#endif
_VOID RTOS_Time_init(_VOID);

//等待	单位毫秒
_VOID waitTime(_UINT p_nMillisecond);

//等待	单位微秒
_VOID waitMicroTime(_UINT p_nMicrosecond);
//获取当前系统时间缓存Buf
_VOID getCurSysTimeBuf(_CHAR* p_szOutBuf, const _INT p_nBufSize, _INT* p_nOutLen);
//获取当前的时间
SysTime_s getCurSysTime(_VOID);
//获取当前系统时间：从1970年开始，受修改时间操作的影响（单位：ms）
_u32 getCurSysTime_ms(_VOID);

//获取当前系统运行时间：从0开始，不收修改时间操作的影响（单位: us）
_u64 getUpTime_us(_VOID);
//获取当前系统时间
_u64 getCurSysTime_us(_VOID);

//以下用于程序运行时间计时(us)
_u64 GetTimeTick(_VOID);
//取当前系统的滴答频率
_DOUBLE GetTickFrequency(void);

#define CurUptime_ms(t)		_u32 t = us2ms(getUpTime_us())

#endif


