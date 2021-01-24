#ifndef _PERFORMANCE_TIME_H_
#define _PERFORMANCE_TIME_H_


#include "TypeDef.h"
#include "SysConfig.h"
#include "SysTime.h"

typedef struct _PerformanceTime_s
{

	_u64	startTime;
	_u64	endTime;

}PerformanceTime_s;


void startTimeTest(PerformanceTime_s* p_pstPerformanceTime);

void endTimeTest(PerformanceTime_s* p_pstPerformanceTime);

_u64 getPassTime(PerformanceTime_s* p_pstPerformanceTime);


#if PERFORMANCE_TIME_ON
#define _PERFORMANCE_(_PerformanceTime_) PerformanceTime_s _PerformanceTime_
#define START_TIME_TEST(_PerformanceTime_)	startTimeTest(&_PerformanceTime_)
#define END_TIME_TEST(_PerformanceTime_)	endTimeTest(&_PerformanceTime_)
#define PASS_TIME(_PerformanceTime_)	getPassTime(&_PerformanceTime_)

#else
#define _PERFORMANCE_(_PerformanceTime_)
#define START_TIME_TEST(_PerformanceTime_)	
#define END_TIME_TEST(_PerformanceTime_)	
#define PASS_TIME(_PerformanceTime_)	0

#endif

typedef enum _TimePrecision_e{
	TimePrecision_1s = 1,
	TimePrecision_100ms,
	TimePrecision_10ms,
	TimePrecision_1ms,
	TimePrecision_100us,
	TimePrecision_10us,
	TimePrecision_1us,	
}TimePrecision_e;

#ifdef WIN32
#define TimePrecision_NOR	TimePrecision_10us		//	常用的时间统计单位
#else
#define TimePrecision_NOR	TimePrecision_1ms
#endif // WIN32



_VOID	PT_printTimeInfo(const _CHAR *p_szTitle, _u64 *p_szTime, _u8 p_u8TimeNum, TimePrecision_e p_eTimePrecision);
_u32	PT_totalTIme(_u64 *p_szTime, _u8 p_u8TimeNum, TimePrecision_e p_eTimePrecision);
#ifdef GSG_DEBUG
#define PTIME_ARR(szTime,nNum)							_u64 szTime[nNum] = {0};_u8 u8PTimeNum = 0
#define PTIME_SAVE(szTime)								szTime[u8PTimeNum++] = getCurSysTime_us()
#define PTIME_PRINT(szTitle,szTime,eTimePrecision)		PT_printTimeInfo(szTitle,szTime,u8PTimeNum,eTimePrecision)
#define PTIME_TOTAL(szTime,eTimePrecision)				PT_totalTIme(szTime,u8PTimeNum,eTimePrecision)
#else
#define PTIME_ARR(szTime,nNum)							
#define PTIME_SAVE(szTime)								
#define PTIME_PRINT(szTitle,szTime,eTimePrecision)		
#define PTIME_TOTAL(szTime,eTimePrecision)				0
#endif

#endif //_PERFORMANCE_TIME_H_

