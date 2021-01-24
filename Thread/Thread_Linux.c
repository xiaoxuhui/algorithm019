#include "Thread.h"
#include "Signal.h"
#include "CodeDef.h"
#include <stdio.h>
#include "SysMemory.h"
#include "SysTime.h"
#include "ThreadWTD.h"

#if  (defined OS_LINUX)

#include <pthread.h>
#include "Signal.h"
#include <sched.h>


typedef enum _SignalThread_Status_e
{
	SignalStatus_Suspend = 0,//信号状态挂起
	SignalStatus_Resume = 1,//信号状态唤醒
}SignalThread_Status_e;

typedef struct _Thread_Status_Linux_s
{
	void *          	  pHandle; //线程句柄
	_BOOL				  bStopFlag;//停止标志
	_BOOL				  bExitFlag;//实际退出标记
	_BOOL				  bInFuncFlag;//进入用户函数标志
	Lock_s				  sLock;//信号锁
	Signal_Thread		  nThreadSignal;//线程信号
	SignalThread_Status_e eSignalStatus;//线程信号状态
	pthread_attr_t		  attr;
	_CHAR*				  pszStackPoint;//栈指针
	_INT				  nThreadID;
	_u8					  u8WTDId;
}Thread_Status_Linux_s;

static _INT		g_nThreadID = 1;
void* ThreadFunc_Signal(void* p_pData)
{
	ThreadInfo_s* pThreadInfo = (ThreadInfo_s*)p_pData;
	CHECK_NULL(pThreadInfo->pHandle, NULL);
	Thread_Status_Linux_s* pStatusLinux = (Thread_Status_Linux_s*)pThreadInfo->pHandle;

	ThreadWTD_enable(pStatusLinux->u8WTDId);
	pStatusLinux->bInFuncFlag = true;
	ThreadFuncRet_e nRet = RET_THREAD_CONTINUE;
	while (nRet == RET_THREAD_CONTINUE)
	{		
		if (THREAD_SIGNAL == pThreadInfo->eThreadMode)
		{
			lock(&pStatusLinux->sLock);
			while (SignalStatus_Suspend == pStatusLinux->eSignalStatus)
			{
				waitSignal(&pStatusLinux->nThreadSignal, &pStatusLinux->sLock);
				//printf("WaiteSignal!!\n");
			}
			unlock(&pStatusLinux->sLock);
		}

		ThreadWTD_run(pStatusLinux->u8WTDId);
		nRet = pThreadInfo->FuncBody(pThreadInfo->pData);
		if (pStatusLinux->bStopFlag)
		{
			break;
		}
	}
	pStatusLinux->bExitFlag = true;

	ThreadWTD_disable(pStatusLinux->u8WTDId);
	return NULL;
}

//创建线程
_BOOL createThread(ThreadInfo_s* p_stThreadInfo)
{
	return true;
}

//销毁线程
_VOID destroyThread(ThreadInfo_s* p_stThreadInfo)
{

}

_VOID setThreadPriority(pthread_attr_t* p_stThreadAttr, const _INT p_nPriority)
{
	struct sched_param param = {0};//计划参数
	int		policy = 0;	//策略
	int		min = 0, max = 0;
	pthread_attr_getschedpolicy(p_stThreadAttr, &policy);
	pthread_attr_setschedpolicy(p_stThreadAttr, SCHED_RR);
	pthread_attr_getschedpolicy(p_stThreadAttr, &policy);
	switch (policy)
	{
	case SCHED_FIFO:
		_PRINT_LN_T("-->current thread use policy is SCHED_FIFO --<");
		break;

	case SCHED_RR:
		_PRINT_LN_T("-->current thread use policy is SCHED_RR --<");
		break;

	case SCHED_OTHER:
		_PRINT_LN_T("-->current thread use policy is SCHED_OTHER --<");
		break;
	default:
		_PRINT_LN_T("-->current thread use policy is UNKNOW --<");
		break;
	}
	min = sched_get_priority_min(policy);
	max = sched_get_priority_max(policy);
	pthread_attr_getschedparam(p_stThreadAttr, &param);
	param.sched_priority = p_nPriority;
	pthread_attr_setschedparam(p_stThreadAttr, &param);
}

_BOOL startThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	p_stThreadInfo->pHandle = (void*)SYS_MALLOC(sizeof(Thread_Status_Linux_s));
	Thread_Status_Linux_s* pStatusLinux = (Thread_Status_Linux_s*)p_stThreadInfo->pHandle;
	pStatusLinux->bStopFlag = false;
	pStatusLinux->bExitFlag = false;
	pStatusLinux->bInFuncFlag = false;
	if (THREAD_SIGNAL == p_stThreadInfo->eThreadMode)
	{
		initLock(&pStatusLinux->sLock);
		initSignal(&pStatusLinux->nThreadSignal);
		pStatusLinux->eSignalStatus = SignalStatus_Suspend;
	}
	pthread_attr_init(&pStatusLinux->attr);
	if (p_stThreadInfo->u16ThreadStackSize != 0){//TODO 为何需要乘以1024?
		pStatusLinux->pszStackPoint = SYS_MALLOC(1024 * p_stThreadInfo->u16ThreadStackSize);
		pthread_attr_setstack(&pStatusLinux->attr, pStatusLinux->pszStackPoint, 1024 * p_stThreadInfo->u16ThreadStackSize);
	}
	if (p_stThreadInfo->nPriority != 0){// TODO 不为0则使用默认级别
		setThreadPriority(&pStatusLinux->attr,p_stThreadInfo->nPriority);
	}
	int      detachState;
	detachState = PTHREAD_CREATE_DETACHED;
	_INT nRet = pthread_attr_setdetachstate(&pStatusLinux->attr, detachState);
	if (nRet != 0) {
		return false;
	}

	pStatusLinux->u8WTDId = ThreadWTD_requestWTDId(p_stThreadInfo->szThreadName);

	pthread_t th;
	_INT ret = pthread_create(&th, &pStatusLinux->attr, ThreadFunc_Signal, (void*)p_stThreadInfo);
	if (ret != 0){
		if (THREAD_SIGNAL == p_stThreadInfo->eThreadMode)
		{
			deleteLock(&pStatusLinux->sLock);
			destorySignal(&pStatusLinux->nThreadSignal);
		}
		if (pStatusLinux->pszStackPoint)
		{
			SYS_FREE(pStatusLinux->pszStackPoint);
			pStatusLinux->pszStackPoint = NULL;
		}
		SYS_FREE(p_stThreadInfo->pHandle);
		return false;
	}
	pthread_setname_np(th, p_stThreadInfo->szThreadName);
	pStatusLinux->pHandle = th;
	pStatusLinux->nThreadID = g_nThreadID++;//给每个线程赋一个独立的ID,


	_PRINT_LN_T("startThread[%s]:%u %u",p_stThreadInfo->szThreadName,pStatusLinux->nThreadID,pStatusLinux->u8WTDId);
	pthread_detach(pStatusLinux->pHandle);
	return true;
}

_VOID stopThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo);
	CHECK_NULL(p_stThreadInfo->pHandle);

	//_PRINT_("stopThread11111 Begin !\n");
	Thread_Status_Linux_s* pStatusLinux = (Thread_Status_Linux_s*)p_stThreadInfo->pHandle;	
	//_PRINT_("stopThread22222 Begin !\n");
	pStatusLinux->bStopFlag = true;
	if (THREAD_SIGNAL == p_stThreadInfo->eThreadMode)
	{
		resumeThread(p_stThreadInfo);
		while (true)
		{
			//当用户线程方法卡死时，必须等用户线程方法执行完或者kill掉用户线程方法，才能退出。
			//如果用户线程方法未执行完，就将锁、信号、结构体释放的话，有概率崩溃。（测试已出现）
			//暂认为不会出现用户线程方法卡死
			if (!pStatusLinux->bInFuncFlag)
			{
				deleteLock(&pStatusLinux->sLock);
				destorySignal(&pStatusLinux->nThreadSignal);
				break;
			}
			if (pStatusLinux->bExitFlag)
			{
				deleteLock(&pStatusLinux->sLock);
				destorySignal(&pStatusLinux->nThreadSignal);
				break;
			}
			waitTime(10);
		}
	}
	//_PRINT_("stopThread333333 Begin !pStatusLinux=%p\n", pStatusLinux);
	_u64  u64CurrentTime = getCurSysTime_ms();
	while (true)
	{
		if (pStatusLinux->bExitFlag || getCurSysTime_ms() - u64CurrentTime > 10000)
		{
			if (getCurSysTime_ms() - u64CurrentTime > 10000)
			{
				_PRINT_LN("Thread:(%s)OverTime!Force Exit!", p_stThreadInfo->szThreadName);
			}
			//_PRINT_LN("stopThread Free Begin bExitFlag=%d!", pStatusLinux->bExitFlag);
			pthread_attr_destroy(&pStatusLinux->attr);
			if (pStatusLinux->pszStackPoint)
			{
				SYS_FREE(pStatusLinux->pszStackPoint);
				pStatusLinux->pszStackPoint = NULL;
			}
			SYS_FREE(p_stThreadInfo->pHandle);
			break;

		}
		waitTime(10);
	}
	//_PRINT_("stopThread444444 Begin !\n");
}

_BOOL resumeThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	CHECK_NULL(p_stThreadInfo->pHandle, false);
	CHECK_ET_VALUE(p_stThreadInfo->eThreadMode, THREAD_SIGNAL, false);
	Thread_Status_Linux_s* pStatusLinux = (Thread_Status_Linux_s*)p_stThreadInfo->pHandle;
	//printf("resumeThread_Signal In!\n");
	if (SignalStatus_Suspend == pStatusLinux->eSignalStatus)
	{
		lock(&pStatusLinux->sLock);
		pStatusLinux->eSignalStatus = SignalStatus_Resume;
		resumeSignal(&pStatusLinux->nThreadSignal);
		unlock(&pStatusLinux->sLock);
	}
	//printf("resumeThread_Signal Out!\n");
	return true;
}
_BOOL suspendThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	CHECK_NULL(p_stThreadInfo->pHandle, false);
	CHECK_ET_VALUE(p_stThreadInfo->eThreadMode, THREAD_SIGNAL, false);
	//printf("SignalThread_Suspend In!\n");
	Thread_Status_Linux_s* pStatusLinux = (Thread_Status_Linux_s*)p_stThreadInfo->pHandle;
	if (SignalStatus_Resume == pStatusLinux->eSignalStatus)
	{
		lock(&pStatusLinux->sLock);
		pStatusLinux->eSignalStatus = SignalStatus_Suspend;
		unlock(&pStatusLinux->sLock);
	}
	//printf("SignalThread_Suspend Out!\n");
	return true;
}

#endif
