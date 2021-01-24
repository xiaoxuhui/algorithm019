#ifndef _SIGN_THREAD_H_	
#define _SIGN_THREAD_H_


#include "TypeDef.h"

#define  GSG_THREAD_DEFAULT_STACK_SIZE	1024 //��λ��k

typedef enum _ThreadFuncRet_e
{
	RET_THREAD_CONTINUE = 0,
	RET_THREAD_BREAK = -1

}ThreadFuncRet_e;

typedef enum _ThreadMode_e
{
	THREAD_NORMAL = 0,//�����̣߳���֧�ֻ��ѻ���
	THREAD_SIGNAL = 1,//�ź��̣߳�֧�ֻ��ѻ���
}ThreadMode_e;

#define THREAD_NAME_LEN	 32

typedef struct _ThreadInfo_s
{
	//�߳�ִ����,�������
	ThreadFuncRet_e(*FuncBody)(_VOID*);

	//�߳���
    _CHAR            szThreadName[THREAD_NAME_LEN];//    //���߳���ʹ�õ���Ϣ���������
	_VOID* pData;
	//�߳�ģʽ.�������
	ThreadMode_e	eThreadMode;
	//�߳�ջ��С(����СΪ0ʱ��ʹ��Ĭ��ջ��С1024)
	_u16			u16ThreadStackSize;
	//�߳����ȼ�
	_INT			nPriority;
	//���������ָ�벻����仯��
	void*	pHandle;
}ThreadInfo_s;

//�����߳�
_BOOL createThread(ThreadInfo_s* p_stThreadInfo);

//�����߳�
_VOID destroyThread(ThreadInfo_s* p_stThreadInfo);


//�����ź��߳�
_BOOL startThread(ThreadInfo_s* p_stThreadInfo);
//�ź��̻߳���
_BOOL resumeThread(ThreadInfo_s* p_stThreadInfo);
//�ź��̹߳���
_BOOL suspendThread(ThreadInfo_s* p_stThreadInfo);
//ֹͣ�ź��߳�
_VOID stopThread(ThreadInfo_s* p_stThreadInfo);

#endif
