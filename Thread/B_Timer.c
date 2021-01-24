#include "CodeDef.h"
#include "B_Timer.h"
#include "BaseErrorDef.h"
#include "SysMemory.h"
#include "SysTime.h"
#include <string.h>
#include <assert.h>
#include "Lock.h"
#ifdef OS_RTOS
#include "Thread.h"
#include "MemoryChecker.h"
#include <ti/sysbios/knl/Task.h>
#define TIMER_STACK_SIZE		950  // modify by xxh at 2017-11-14 ，vsprintf和ISC_createAreaFeature_s分别减小了250和136个字节的栈空间
#else
#include "Thread.h"
#endif

//#undef NULL
//#define NULL 0
#define InvalidTaskId		0

typedef struct _ITimerTask_s
{
	_u64			u64NextExeTime;		//下次执行时间
	TimerTask_s		stTask;
}ITimerTask_s;

typedef struct _BTimer_s
{
	_u8				u8ValidTaskNum;		//有效任务数
	_u8				u8MinInterval;		//最小间隔
	_u8				u8MaxTaskNum;		//最大任务数
	ITimerTask_s	*pstTaskList;		//任务列表

#ifdef OS_RTOS
	Task_Struct		stTaskStruct;
	_CHAR			szStack[TIMER_STACK_SIZE];
#else
	ThreadInfo_s	stThread;			//不同平台实现不一样	
#endif

	Lock_s			stLock;
}BTimer_s;

static  BTimer_s		*g_pstBTimer = NULL;

static _u32 Timer_getTaskId(_VOID)
{//范围是1
	static _u32 g_u32TaskId = 1;
	_u32 u32TaskId = g_u32TaskId;
	g_u32TaskId++;
	return u32TaskId;
}

#define REMOVE_TASK(pstTimer,pstTask) do \
{\
	pstTask->stTask.u32TaskId = InvalidTaskId;\
	pstTimer->u8ValidTaskNum--;\
} while (false)

ThreadFuncRet_e Timer_run(_VOID * arg)
{//
	BTimer_s *pstTimer = g_pstBTimer;
	_u64 u64CurrentTime = getCurSysTime_us();
	for (int i = 0; i < pstTimer->u8MaxTaskNum; i++)
	{
		ITimerTask_s *pstTask = pstTimer->pstTaskList + i;
		if (pstTask->stTask.u32TaskId == InvalidTaskId){//无效的任务
			continue;
		}		
		if (u64CurrentTime < pstTask->u64NextExeTime){
			continue;
		}
		TimerTaskRet_e eRet = pstTask->stTask.funTask(pstTask->stTask.pData);
		if (eRet == TimerTaskRet_Break){//移除定时任务
			lock(&g_pstBTimer->stLock);
			REMOVE_TASK(pstTimer, pstTask);
			unlock(&g_pstBTimer->stLock);
		}
		else{//计算下次执行时间
			pstTask->u64NextExeTime += pstTask->stTask.u32Interval * 1000;
		}
	}
	waitTime(pstTimer->u8MinInterval);
	return RET_THREAD_CONTINUE;
}

_VOID	Timer_run_whole(_uArg arg0, _uArg arg1)
{
	ThreadFuncRet_e nRet = RET_THREAD_CONTINUE;
	while (nRet == RET_THREAD_CONTINUE)
	{
#ifdef OS_RTOS
		TASK_MEMORY_DEFAULT_CHECK((_u8*)g_pstBTimer->szStack);
#endif
		nRet = Timer_run(NULL);		
	}
}

//p_u8MinInterval 最小间隔周期，返回句柄
_BOOL B_Timer_init(const _u8 p_u8MinInterval, const _u8 p_u8MaxTaskNum)
{
	INIT_STATIC_PARAM(g_pstBTimer, BTimer_s, false);
	initLock(&g_pstBTimer->stLock);

	g_pstBTimer->u8MaxTaskNum = p_u8MaxTaskNum;
	g_pstBTimer->pstTaskList = SYS_MALLOC(sizeof(ITimerTask_s)* p_u8MaxTaskNum);
	CHECK_NULL(g_pstBTimer->pstTaskList, false);
	memset(g_pstBTimer->pstTaskList, 0, sizeof(ITimerTask_s)* p_u8MaxTaskNum);

	g_pstBTimer->u8MinInterval = p_u8MinInterval;
	g_pstBTimer->u8ValidTaskNum = 0;

#ifdef OS_RTOS
	Task_Params taskParams;
	Task_Params_init(&taskParams);
	taskParams.stackSize = TIMER_STACK_SIZE;
	taskParams.stack = &g_pstBTimer->szStack;
	taskParams.priority = 1;
	Task_construct(&g_pstBTimer->stTaskStruct, Timer_run_whole, &taskParams, NULL);
	TASK_MEMORY_INIT((_u8*)g_pstBTimer->szStack);
	

	
#else
	g_pstBTimer->stThread.eThreadMode = THREAD_NORMAL;
	g_pstBTimer->stThread.FuncBody = Timer_run;
	strcpy(g_pstBTimer->stThread.szThreadName, "BTimer");
	_BOOL bRet = createThread(&g_pstBTimer->stThread);
	CHECK_ET_VALUE(bRet, true, false);

	bRet = startThread(&g_pstBTimer->stThread);
	CHECK_ET_VALUE(bRet, true, false);
#endif
	return true;
}

_VOID B_Timer_uninit(_VOID)
{
	BTimer_s *pstTimer = g_pstBTimer;
#ifdef OS_RTOS


#else
	stopThread(&pstTimer->stThread);
	destroyThread(&pstTimer->stThread);
	
#endif
	SYS_FREE(pstTimer->pstTaskList);
	SYS_FREE(pstTimer);	
}

//添加一个定时任务
_INT B_Timer_addTask(TimerTask_s *p_pstTask)
{
	CHECK_NULL(p_pstTask, RET_PARAM_ERROR);
	BTimer_s *pstTimer = g_pstBTimer;
	lock(&g_pstBTimer->stLock);
	if (pstTimer->u8ValidTaskNum >= pstTimer->u8MaxTaskNum){
		unlock(&g_pstBTimer->stLock);
		return RET_OVER_MAX_LIMIT;
	}

	_INT nRet = RET_OVER_MAX_LIMIT;
	for (int i = 0; i < pstTimer->u8MaxTaskNum; i++)
	{
		ITimerTask_s *pstTask = pstTimer->pstTaskList + i;
		if (pstTask->stTask.u32TaskId != InvalidTaskId){
			continue;
		}
		p_pstTask->u32TaskId = Timer_getTaskId();
		pstTask->stTask = *p_pstTask;
		_u64 u64CurrentTime = getCurSysTime_us();
		pstTask->u64NextExeTime = u64CurrentTime + pstTask->stTask.u32FirstTime * 1000;
		pstTimer->u8ValidTaskNum++;
		nRet = RET_SUCCESS;
		break;
	}
	unlock(&g_pstBTimer->stLock);
	assert(nRet == RET_SUCCESS);
	return nRet;
}

_INT B_Timer_removeTask(_u32 p_u32TaskId)
{
	lock(&g_pstBTimer->stLock);
	BTimer_s *pstTimer = g_pstBTimer;
	_INT nRet = RET_NOT_FIND;
	for (int i = 0; i < pstTimer->u8MaxTaskNum; i++)
	{
		ITimerTask_s *pstTask = pstTimer->pstTaskList + i;
		if (pstTask->stTask.u32TaskId != p_u32TaskId){
			continue;
		}
		REMOVE_TASK(pstTimer, pstTask);
		nRet = RET_SUCCESS;
		break;
	}
	unlock(&g_pstBTimer->stLock);
	return nRet;
}
