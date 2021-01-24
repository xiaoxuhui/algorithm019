#ifndef _B_TIMER_H_
#define _B_TIMER_H_

#include "Thread.h"
#include "TypeDef.h"
#include "CodeDef.h"
#include "GSG.h"
#ifdef OS_NRF
#include "nrf_drv_timer.h"
#endif

typedef enum _TimerTaskRet_e
{
	TimerTaskRet_Continue = 0,		//����
	TimerTaskRet_Break = 1			//ֹͣ
}TimerTaskRet_e;

typedef TimerTaskRet_e(TimerTaskFun)(_VOID* pData);

typedef struct  _TimerTask_s
{
	_u32			u32TaskId;
	_u32			u32FirstTime;	//�״�ִ������ʱ�䣬����ӿ�ʼ��ʱ��0��ʾ��������
	_u32			u32Interval;	//��ʱ���ڣ���ÿ������ִ��һ������
	TimerTaskFun	*funTask;		//��ʱ����
	_VOID			*pData;			//��ʱ����Ĳ���
}TimerTask_s;

#ifdef OS_NRF
_VOID RCMCU_Timer_setTimerCfg(nrf_drv_timer_config_t *pTimer_cfg);
_VOID RC_MCU_Timer2_Init(const _u8 _pu8TIME_ms);
#endif

_BOOL B_Timer_init(const _u8 p_u8MinInterval, const _u8 p_u8MaxTaskNum);
_VOID B_Timer_uninit(_VOID);
//���һ����ʱ����
_INT B_Timer_addTask(TimerTask_s *p_pstTask);
_INT B_Timer_removeTask(_u32 p_u32TaskId);

#endif	//_B_TIMER_H_
