#ifndef _SIGN_THREAD_H_	
#define _SIGN_THREAD_H_


#include "TypeDef.h"

#define  GSG_THREAD_DEFAULT_STACK_SIZE	1024 //单位：k

typedef enum _ThreadFuncRet_e
{
	RET_THREAD_CONTINUE = 0,
	RET_THREAD_BREAK = -1

}ThreadFuncRet_e;

typedef enum _ThreadMode_e
{
	THREAD_NORMAL = 0,//正常线程，不支持唤醒机制
	THREAD_SIGNAL = 1,//信号线程，支持唤醒机制
}ThreadMode_e;

#define THREAD_NAME_LEN	 32

typedef struct _ThreadInfo_s
{
	//线程执行体,输入参数
	ThreadFuncRet_e(*FuncBody)(_VOID*);

	//线程名
    _CHAR            szThreadName[THREAD_NAME_LEN];//    //给线程体使用的信息。输入参数
	_VOID* pData;
	//线程模式.输入参数
	ThreadMode_e	eThreadMode;
	//线程栈大小(当大小为0时，使用默认栈大小1024)
	_u16			u16ThreadStackSize;
	//线程优先级
	_INT			nPriority;
	//输出参数，指针不允许变化。
	void*	pHandle;
}ThreadInfo_s;

//创建线程
_BOOL createThread(ThreadInfo_s* p_stThreadInfo);

//销毁线程
_VOID destroyThread(ThreadInfo_s* p_stThreadInfo);


//开启信号线程
_BOOL startThread(ThreadInfo_s* p_stThreadInfo);
//信号线程唤醒
_BOOL resumeThread(ThreadInfo_s* p_stThreadInfo);
//信号线程挂起
_BOOL suspendThread(ThreadInfo_s* p_stThreadInfo);
//停止信号线程
_VOID stopThread(ThreadInfo_s* p_stThreadInfo);

#endif
