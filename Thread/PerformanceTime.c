#include "PerformanceTime.h"
#include "SysTime.h"
#include "MathGSG.h"
#include "CodeDef.h"


void startTimeTest(PerformanceTime_s* p_pstPerformanceTime)
{
	p_pstPerformanceTime->startTime = GetTimeTick();
}

void endTimeTest(PerformanceTime_s* p_pstPerformanceTime)
{
	p_pstPerformanceTime->endTime = GetTimeTick();
}

#ifdef WIN32
#include <windows.h>
#include <time.h>

_u64 getPassTime(PerformanceTime_s* p_pstPerformanceTime)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	_DOUBLE time = (_DOUBLE)(p_pstPerformanceTime->endTime - p_pstPerformanceTime->startTime) / (_DOUBLE)freq.QuadPart;

	return (_u64)(time * 1000000);
}


#else

_u64 getPassTime(PerformanceTime_s* p_pstPerformanceTime)
{
	return p_pstPerformanceTime->endTime - p_pstPerformanceTime->startTime;
}


#endif


_s32 PT_calcPrecision(TimePrecision_e p_eTimePrecision)
{
	switch (p_eTimePrecision)
	{
	case TimePrecision_1s:
		return 1000000;
	case TimePrecision_100ms:
		return 100000;
	case TimePrecision_10ms:
		return 10000;
	case TimePrecision_1ms:
		return 1000;
	case TimePrecision_100us:
		return 100;
	case TimePrecision_10us:
		return 10;
	case TimePrecision_1us:
		return 1;
	default:
		DBG_ASSERT(0);
		return 0;
	}
}

_CHAR *PT_precisionName(TimePrecision_e p_eTimePrecision)
{
	switch (p_eTimePrecision)
	{
	case TimePrecision_1s:
		return "1s";
	case TimePrecision_100ms:
		return "100ms";
	case TimePrecision_10ms:
		return "10ms";
	case TimePrecision_1ms:
		return "1ms";
	case TimePrecision_100us:
		return "100us";
	case TimePrecision_10us:
		return "10us";
	case TimePrecision_1us:
		return "1us";
	default:
		DBG_ASSERT(0);
		return 0;
	}
}

_VOID	PT_printTimeInfo(const _CHAR *p_szTitle, _u64 *p_szTime, _u8 p_u8TimeNum, TimePrecision_e p_eTimePrecision)
{
	DBG_ASSERT(p_u8TimeNum > 1);
	_s32	s32Precision_us = PT_calcPrecision(p_eTimePrecision);

	_B_PRINT_TIME();
	_PRINT_("WT_%s(%u %s):", p_szTitle, PT_totalTIme(p_szTime, p_u8TimeNum, p_eTimePrecision), PT_precisionName(p_eTimePrecision));
	for (_INT i = 0; i < p_u8TimeNum - 1; i++){
		_s32 s32DeltaTime = (_s32)(p_szTime[i + 1] - p_szTime[i]);
		_u32 u32Time_ms = (_u32)GSG_div(s32DeltaTime, s32Precision_us);
		_PRINT_("%u\t", u32Time_ms);
	}
	_PRINT_LN("");
}

_u32	PT_totalTIme(_u64 *p_szTime, _u8 p_u8TimeNum, TimePrecision_e p_eTimePrecision)
{
	DBG_ASSERT(p_u8TimeNum > 1);
	_s32	s32Precision_us = PT_calcPrecision(p_eTimePrecision);
	_s32 s32DeltaTime = (_s32)(p_szTime[p_u8TimeNum - 1] - p_szTime[0]);
	return (_u32)GSG_div(s32DeltaTime, s32Precision_us);
}
