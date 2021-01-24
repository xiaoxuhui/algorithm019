#include "SysTime.h"


//52832//OS_NRF SYSTICK_ENABLED BOARD_PCA10040 BSP_DEFINES_ONLY CONFIG_GPIO_AS_PINRESET FLOAT_ABI_HARD NRF52 NRF52832_XXAA NRF52_PAN_74
#ifdef WIN32

#include <windows.h>
#include <time.h>
#include <stdio.h>
void waitTime(_UINT p_nMillisecond)
{
	Sleep(p_nMillisecond);
}

_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	_UINT nMillisecond = p_nMicrosecond / 1000;
	_UINT nMicrosecond = p_nMicrosecond % 1000;

	waitTime(nMillisecond);

	LARGE_INTEGER m_liPerfFreq = { 0 };

	if (!QueryPerformanceFrequency(&m_liPerfFreq))
	{
		//cout << "你的当前计算机硬件不支持高精度计时器" << endl;
		return;
	}

	LARGE_INTEGER m_liPerfStart = { 0 };
	QueryPerformanceCounter(&m_liPerfStart);

	LARGE_INTEGER liPerfNow = { 0 };
	for (;;)
	{

		QueryPerformanceCounter(&liPerfNow);
		double time = (((liPerfNow.QuadPart -
			m_liPerfStart.QuadPart) * 1000000) / (double)m_liPerfFreq.QuadPart);
		if (time >= nMicrosecond)
			break;

	}

}

_VOID getCurSysTimeBuf(_CHAR* p_szOutBuf, const _INT p_nBufSize, _INT* p_nOutLen)
{
	SysTime_s curTime = getCurSysTime();
	sprintf(p_szOutBuf, "%04d%02d%02d%02d%02d%02d", curTime.year, curTime.month, curTime.day, curTime.hour, curTime.minute, curTime.second);
	*p_nOutLen = strlen(p_szOutBuf);
}


_u64 GetTimeTick(_VOID)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (_u64)counter.QuadPart;
}

_DOUBLE GetTickFrequency(void)
{
	_DOUBLE dFre = 1.0;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	dFre = (_DOUBLE)freq.QuadPart;

	return (dFre * 1e-6);
}
SysTime_s getCurSysTime(_VOID)
{
	SysTime_s retTime;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	retTime.year = sys.wYear;
	retTime.month = sys.wMonth;
	retTime.day = sys.wDay;
	retTime.hour = sys.wHour;
	retTime.minute = sys.wMinute;
	retTime.second = sys.wSecond;
	retTime.millisecond = sys.wMilliseconds;
	retTime.dayOfWeek = sys.wDayOfWeek;

	retTime.UTCSecond = (_u64)time(0);		//1970到现在的秒数

	return retTime;
}

_u64 GetTimeTick_Up()
{
	return getCurSysTime_us();
}

#elif defined OS_RTOS
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include "SpinLock.h"

//{ modify by xxh at 2017-11-18 解决Clock_getTicks最多只能表示32位的问题，一旦回滚则自动增加u32
static _u32 g_u32Ticks_last = 0;
static _u64 u64Ticks_base = 0;

static SpinLock_s g_stSpinLock;

_VOID RTOS_Time_init(_VOID)
{
	initSpinLock(&g_stSpinLock);
}

_u64 GetTimeTick_Up()
{
	spin_lock_interrupt(&g_stSpinLock);
	_u32 u32Ticks_Cur = Clock_getTicks();

	_u32 u32Ticks_last = g_u32Ticks_last; // 此处代码是为了防止多线程访问而导致u64Ticks_base多加1次
	g_u32Ticks_last = u32Ticks_Cur;

	if(u32Ticks_Cur < u32Ticks_last){
		u64Ticks_base += ((_u64)1 << 32);
	}
	_u64 u64CurTime = u32Ticks_Cur + u64Ticks_base;
	spin_unlock_interrupt(&g_stSpinLock);

	return u64CurTime;
}
//} end

_VOID waitTime(_UINT p_nMillisecond)
{
	Task_sleep(p_nMillisecond*1000);
}

_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	Task_sleep(p_nMicrosecond);
}

SysTime_s getCurSysTime(_VOID)
{
   SysTime_s retTime = {0};

	//先空实现
	return retTime;
}

_u64 GetTimeTick()
{
	return GetTimeTick_Up();
}

_DOUBLE GetTickFrequency(void)
{
	return 1;
}
#elif  defined OS_LINUX_DRIVER
#include <linux/delay.h>
#include <linux/time.h>

_VOID waitTime(_UINT p_nMillisecond)
{
	msleep(p_nMillisecond);
}

_u64 GetTimeTick()
{
	_u64 tmp = 0;
	struct timeval tv;
	do_gettimeofday(&tv);
	tmp = (_u64)tv.tv_sec;
	tmp = tmp * 1000000 + tv.tv_usec;		//防止乘法越界
	return tmp;
}
_u64 GetTimeTick_Up()
{
	return GetTimeTick();
}
_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	udelay(p_nMicrosecond);
}
SysTime_s getCurSysTime(_VOID)
{
	SysTime_s retTime = {0};

	//先空实现
	return retTime;
}
_DOUBLE GetTickFrequency(void)
{
	return 1;
}
_u32 getCurSysTime_ms(_VOID)
{
	_u32 tmp = 0;
	struct timeval tv;
	do_gettimeofday(&tv);
	tmp = (_u32)tv.tv_sec;
	tmp = tmp * 1000 + tv.tv_usec/1000;		//防止乘法越界
	return tmp;
}

_u64 getCurSysTime_us(_VOID)
{
	return GetTimeTick();
}
_u64 getUpTime_us(_VOID)
{
	return getCurSysTime_us();
}
#elif  defined OS_LITEOS

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

_VOID waitTime(_UINT p_nMillisecond)
{
	usleep(p_nMillisecond*10000);
}

_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	usleep(p_nMicrosecond*10);
}

_u64 GetTimeTick()
{
	_u64 tmp = 0;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	tmp = (_u64)tv.tv_sec;
	tmp = tmp * 1000000 + tv.tv_usec;		//防止乘法越界
	return tmp;
}
_DOUBLE GetTickFrequency(void)
{
	_DOUBLE dFre = 1.0;
	dFre = 1e6;
	return (dFre * 1e-6);
}

SysTime_s getCurSysTime()
{
	SysTime_s retTime;

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	time_t lt = time(NULL);
	struct tm * ptr = localtime(&lt);

	retTime.year = ptr->tm_year + 1900;
	retTime.month = ptr->tm_mon + 1;
	retTime.day = ptr->tm_mday;
	retTime.hour = ptr->tm_hour;
	retTime.minute = ptr->tm_min;
	retTime.second = ptr->tm_sec;
	retTime.millisecond = tv.tv_usec / 1000;
	retTime.dayOfWeek = ptr->tm_wday;

	retTime.UTCSecond = (_u64)lt;		//1970到现在的秒数

	return retTime;
}

_u64 GetTimeTick_Up()
{
	return getCurSysTime_us();
}

#elif defined OS_NRF
#include <time.h>
#include "SysPrint.h"
#include "nrf_delay.h"
#include "CodeDef.h"
#ifdef OS_NRF52810

#else
const nrf_drv_timer_t NRF_TIMER_2 = NRF_DRV_TIMER_INSTANCE(4);
nrf_drv_timer_config_t timer_cfg_4 = NRF_DRV_TIMER_DEFAULT_CONFIG;
nrf_drv_timer_config_t *g_pTimer_cfg_2 = NULL;
#endif
#define MAX_TIME_TICK				0x80000000
static _u64 g_u64TickCount_base = 0;

_VOID RTC_callback(_VOID)
{}
	
void NRFTimer_callback(nrf_timer_event_t event_type, void* p_context)
{
		g_u64TickCount_base += MAX_TIME_TICK;	
//	_PRINT_LN_T("TC:%d", _u32)(g_u64TickCount_base >> 31));	
}

_VOID RTOS_Time_setTimeConfig(nrf_drv_timer_config_t *p_pstTimer_cfg)
{
//	g_pTimer_cfg_2 = p_pstTimer_cfg;

}

#ifdef OS_NRF52810
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_clock.h"
#include "boards.h"
#include "app_error.h"
#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_rtc.h"
_u64 g_u64SysCount_ms =0;
//#define  RTC_TICK_US    32.768
static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{

	//_PRINT_LN("rtc_handler ");
	g_u64SysCount_ms++;//32768/33=992.96us进入一次中断

}

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */
static void lfclk_config(void)
{
//    ret_code_t err_code = nrf_drv_clock_init();
//    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}
#endif
_VOID RTOS_Time_init(_VOID)
{
#ifdef OS_NRF52810
	  uint32_t err_code;
	
    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = 32;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);
	
    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,true);
	
    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
	
    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
	
		lfclk_config();
	
	
#else
	timer_cfg_4.interrupt_priority = 2;
	timer_cfg_4.mode = NRF_TIMER_MODE_TIMER;
	_u32 u32Ret = nrf_drv_timer_init(&NRF_TIMER_2, &timer_cfg_4, NRFTimer_callback);		
	DBG_ASSERT(u32Ret == NRFX_SUCCESS);
	nrf_drv_timer_extended_compare(&NRF_TIMER_2, NRF_TIMER_CC_CHANNEL1, MAX_TIME_TICK - 1, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);	
	nrf_drv_timer_enable(&NRF_TIMER_2);	
#endif
}

_VOID waitTime(_UINT p_nMillisecond)
{
	nrf_delay_ms(p_nMillisecond);
}

_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	nrf_delay_us(p_nMicrosecond);
}

SysTime_s getCurSysTime(_VOID)
{
   SysTime_s retTime = {0};

	//先空实现
	return retTime;
}

_u64 GetTimeTick()
{
#ifdef OS_NRF52810
	
#else
	_u32 nValue = nrfx_timer_capture(&NRF_TIMER_2, NRF_TIMER_CC_CHANNEL0);	
	return (nValue + g_u64TickCount_base) / 16;// 16M 时钟, 转换为us
	#endif
}

_DOUBLE GetTickFrequency(void)
{
	return 1;
}

#else
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#ifdef __MAC_OS__
#include <string.h>
#endif
_VOID waitTime(_UINT p_nMillisecond)
{
	usleep(p_nMillisecond * 1000);
}

_VOID waitMicroTime(_UINT p_nMicrosecond)
{
	usleep(p_nMicrosecond);
}

_u64 GetTimeTick()
{
	_u64 tmp = 0;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	tmp = (_u64)tv.tv_sec;
	tmp = tmp * 1000000 + tv.tv_usec;		//防止乘法越界
	return tmp;
}
_DOUBLE GetTickFrequency(void)
{
	_DOUBLE dFre = 1.0;
	dFre = 1e6;
	return (dFre * 1e-6);
}

SysTime_s getCurSysTime()
{
	SysTime_s retTime;

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	time_t lt = time(NULL);
	struct tm * ptr = localtime(&lt);

	retTime.year = ptr->tm_year + 1900;
	retTime.month = ptr->tm_mon + 1;
	retTime.day = ptr->tm_mday;
	retTime.hour = ptr->tm_hour;
	retTime.minute = ptr->tm_min;
	retTime.second = ptr->tm_sec;
	retTime.millisecond = tv.tv_usec / 1000;
	retTime.dayOfWeek = ptr->tm_wday;

	retTime.UTCSecond = (_u64)lt;		//1970到现在的秒数

	return retTime;
}
_VOID getCurSysTimeBuf(_CHAR* p_szOutBuf, const _INT p_nBufSize, _INT* p_nOutLen)
{
	SysTime_s curTime = getCurSysTime();
	sprintf(p_szOutBuf, "%04d%02d%02d%02d%02d%02d", curTime.year, curTime.month, curTime.day, curTime.hour, curTime.minute, curTime.second);
	*p_nOutLen = (_INT)strlen(p_szOutBuf);
}

_u64 GetTimeTick_Up()
{
	return getCurSysTime_us();
}
#endif // WIN32

#ifndef OS_LINUX_DRIVER
_u32 getCurSysTime_ms(_VOID)
{
#ifdef OS_NRF52810
	return (_u32)g_u64SysCount_ms;
#else
	return ((_u32)(getCurSysTime_us() / 1000.0));
#endif
}

_u64 getCurSysTime_us(_VOID)
{
#ifdef OS_NRF52810
	return getCurSysTime_ms()*1000;
#else
	
	_u64 u64CurTime = (_u64)(GetTimeTick() / GetTickFrequency());
	return u64CurTime;
	#endif
}

_u64 getUpTime_us(_VOID)
{
	return getCurSysTime_us();
}
#endif
