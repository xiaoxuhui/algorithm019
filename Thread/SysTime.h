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
	_UINT		dayOfWeek;		//0-6	0��������
	_u64		UTCSecond;		//1970�����ڵ�����

}SysTime_s;

#define us2ms(t)    (_u32)(t/1000)

#ifdef OS_NRF
#include "nrf_drv_timer.h"
_VOID RTOS_Time_setTimeConfig(nrf_drv_timer_config_t *p_pstTimer_cfg);
#endif
_VOID RTOS_Time_init(_VOID);

//�ȴ�	��λ����
_VOID waitTime(_UINT p_nMillisecond);

//�ȴ�	��λ΢��
_VOID waitMicroTime(_UINT p_nMicrosecond);
//��ȡ��ǰϵͳʱ�仺��Buf
_VOID getCurSysTimeBuf(_CHAR* p_szOutBuf, const _INT p_nBufSize, _INT* p_nOutLen);
//��ȡ��ǰ��ʱ��
SysTime_s getCurSysTime(_VOID);
//��ȡ��ǰϵͳʱ�䣺��1970�꿪ʼ�����޸�ʱ�������Ӱ�죨��λ��ms��
_u32 getCurSysTime_ms(_VOID);

//��ȡ��ǰϵͳ����ʱ�䣺��0��ʼ�������޸�ʱ�������Ӱ�죨��λ: us��
_u64 getUpTime_us(_VOID);
//��ȡ��ǰϵͳʱ��
_u64 getCurSysTime_us(_VOID);

//�������ڳ�������ʱ���ʱ(us)
_u64 GetTimeTick(_VOID);
//ȡ��ǰϵͳ�ĵδ�Ƶ��
_DOUBLE GetTickFrequency(void);

#define CurUptime_ms(t)		_u32 t = us2ms(getUpTime_us())

#endif


