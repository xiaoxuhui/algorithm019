#include "Thread.h"

#ifdef  OS_LINUX_DRIVER

#include "SysMemory.h"
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/sched.h>
typedef struct _Thread_Status_Linux_Driver_s
{
	struct task_struct*	pTaskStruct;//ָ��Task��ָ��
	_BOOL				  bStopFlag;//ֹͣ��־
	_BOOL				  bExitFlag;//ʵ���˳����
	_BOOL				  bInFuncFlag;//�����û�������־
}Thread_Status_Linux_Driver_s;

void* ThreadFunc_Main(void* p_pData)
{
	ThreadInfo_s* pThreadInfo = (ThreadInfo_s*)p_pData;
	CHECK_NULL(pThreadInfo->pHandle, NULL);
	Thread_Status_Linux_Driver_s* pStatusLinuxDriver = (Thread_Status_Linux_Driver_s*)pThreadInfo->pHandle;
	pStatusLinuxDriver->bInFuncFlag = true;
	ThreadFuncRet_e nRet = RET_THREAD_CONTINUE;
	while (nRet == RET_THREAD_CONTINUE)
	{
		nRet = pThreadInfo->FuncBody(pThreadInfo->pData);
		if (pStatusLinuxDriver->bStopFlag)
		{
			break;
		}
	}
	pStatusLinuxDriver->bExitFlag = true;
	return NULL;
}
_BOOL startThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo, false);
	p_stThreadInfo->pHandle = (void*)SYS_MALLOC(sizeof(Thread_Status_Linux_Driver_s));
	Thread_Status_Linux_Driver_s* pStatusLinuxDriver = (Thread_Status_Linux_Driver_s*)p_stThreadInfo->pHandle;
	pStatusLinuxDriver->bStopFlag = false;
	pStatusLinuxDriver->bExitFlag = false;
	pStatusLinuxDriver->bInFuncFlag = false;
	pStatusLinuxDriver->pTaskStruct = kthread_create(&ThreadFunc_Main, (void*)p_stThreadInfo, p_stThreadInfo->szThreadName);
	if(IS_ERR(pStatusLinuxDriver->pTaskStruct))
	{
		pStatusLinuxDriver->pTaskStruct = NULL;

		return false;
	}
	//���ѽ���
	wake_up_process(pStatusLinuxDriver->pTaskStruct);
	return true;
}

//�ź��̻߳���
_BOOL resumeThread(ThreadInfo_s* p_stThreadInfo)
{
	return true;
}
//�ź��̹߳���
_BOOL suspendThread(ThreadInfo_s* p_stThreadInfo)
{
	return true;
}
//ֹͣ�ź��߳�
_VOID stopThread(ThreadInfo_s* p_stThreadInfo)
{
	CHECK_NULL(p_stThreadInfo);
	Thread_Status_Linux_Driver_s* pThreadDriver = (Thread_Status_Linux_Driver_s*)(p_stThreadInfo->pHandle);
	pThreadDriver->bStopFlag = true;
	if(pThreadDriver->pTaskStruct&&!pThreadDriver->bExitFlag)
	{
		kthread_stop(pThreadDriver->pTaskStruct);
		pThreadDriver->pTaskStruct = NULL;
	}
	_u64  u64CurrentTime = getCurSysTime_ms();
	while (true)
	{
		if (pThreadDriver->bExitFlag || getCurSysTime_ms() - u64CurrentTime > 10000)
		{
			//_PRINT_LN("stopThread Free Begin bExitFlag=%d!", pStatusLinux->bExitFlag);
			SYS_FREE(p_stThreadInfo->pHandle);
			break;

		}
		waitTime(10);
	}

}


#endif