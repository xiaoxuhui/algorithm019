#include "Thread.h"
#include "Signal.h"
#include "CodeDef.h"
#include <stdio.h>
#include "SysMemory.h"
#include "SysTime.h"
#ifdef WIN32

#include <Windows.h>
#include <process.h>


typedef struct _Thread_Status_Win_s
{
	HANDLE	pHandle;//Handle信息
	_BOOL	bStopFlag;//停止标志
	_INT	nSuspend;//挂起次数，唤醒时减一1，挂起时加一
}Thread_Status_Win_s;

void ThreadFunc_Signal(void* p_pData)
{
	CHECK_NULL(p_pData);
	ThreadInfo_s* pThreadInfo = (ThreadInfo_s*)p_pData;
	Thread_Status_Win_s* pStatus = (Thread_Status_Win_s*)pThreadInfo->pHandle;
	ThreadFuncRet_e nRet = RET_THREAD_CONTINUE;
	while (nRet == RET_THREAD_CONTINUE)
	{
		nRet = pThreadInfo->FuncBody(pThreadInfo->pData);

		if (pStatus->bStopFlag)
		{
			break;
		}
	}
	pStatus->bStopFlag = true;
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


_BOOL startThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	p_stThreadInfo->pHandle = (void*)SYS_MALLOC(sizeof(Thread_Status_Win_s));
	CHECK_NULL(p_stThreadInfo->pHandle, false);
	Thread_Status_Win_s* pStatus = (Thread_Status_Win_s*)p_stThreadInfo->pHandle;
	pStatus->nSuspend = 0;
	pStatus->bStopFlag = false;
	_INT nRet = (_INT)_beginthread(ThreadFunc_Signal, 0, (void*)p_stThreadInfo);
	if (-1 == nRet)
	{
		SYS_FREE(p_stThreadInfo->pHandle);
		return false;
	}
	pStatus->pHandle = (HANDLE)nRet;
	return true;
}

_VOID stopThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo);
	CHECK_NULL(p_stThreadInfo->pHandle);
	Thread_Status_Win_s* pStatus = (Thread_Status_Win_s*)p_stThreadInfo->pHandle;
	pStatus->bStopFlag = true;
	DWORD ret = STILL_ACTIVE;
	while (pStatus->nSuspend > 0)
	{
		int nLoopEnd = pStatus->nSuspend;
		for (int nLoop = 0; nLoop < nLoopEnd; nLoop++)
		{
			resumeThread(p_stThreadInfo);
		}
	}
	while (true)
	{
		BOOL bRet = GetExitCodeThread((void*)pStatus->pHandle, &ret);
		if ((0 == bRet)
			|| (ret != STILL_ACTIVE)
			)
		{
			break;
		}
	}
	SYS_FREE(p_stThreadInfo->pHandle);
}
static int g_nSuspendCount = 0;
static int g_nResumeCount = 0;
_BOOL resumeThread(ThreadInfo_s* p_stThreadInfo)
{

	CHECK_NULL(p_stThreadInfo, false);
	CHECK_NULL(p_stThreadInfo->pHandle, false);
	CHECK_ET_VALUE(p_stThreadInfo->eThreadMode, THREAD_SIGNAL, false);

	g_nResumeCount++;
	Thread_Status_Win_s* pStatus = (Thread_Status_Win_s*)p_stThreadInfo->pHandle;
	pStatus->nSuspend--;
	ResumeThread(pStatus->pHandle);
	return true;
}
_BOOL suspendThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	CHECK_NULL(p_stThreadInfo->pHandle, false);
	CHECK_ET_VALUE(p_stThreadInfo->eThreadMode, THREAD_SIGNAL, false);
	g_nSuspendCount++;
	Thread_Status_Win_s* pStatus = (Thread_Status_Win_s*)p_stThreadInfo->pHandle;
	pStatus->nSuspend++;
	SuspendThread(pStatus->pHandle);
	return true;
}
#endif