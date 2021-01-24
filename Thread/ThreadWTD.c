#include "ThreadWTD.h"
#include "BaseErrorDef.h"
#include "Lock.h"
#include "CodeDef.h"

#define WTD_FLAG_ENABLE		0x5A
#define WTD_FLAG_DISABLE	0xAA
#define WTD_FLAG_PAUSE		0x55
#define WTD_FLAG_INVLAID	0x0
#define WTD_ID_INVALID		0xFF

#define MAX_LEN_WTD_THREAD_NAME	10
typedef struct _ThreadWTD_s
{// 1���߳̿��Ź���Ϣ
	_u8		u8ValidFlag;							//	��Ч��־
	_u32	u32ActiveTime;							//	�ʱ��
	_CHAR	szThreadName[MAX_LEN_WTD_THREAD_NAME];	//	�߳���	
}ThreadWTD_s;

typedef struct _ThreadWTD_Mgr_s
{
	_u8				u8ThreadNum;		//	�߳�����
	_u16			u16TimeOut;			//	��ʱʱ��,��λ: ms
	ThreadWTD_s		*szThreadWTD;		//	�±꼴��WTDId	
	Lock_s			stLock;
}ThreadWTD_Mgr_s;

static ThreadWTD_Mgr_s *g_pstThreadWTDMgr = NULL;

//	��ʼ���߳̿��Ź�
_BOOL	ThreadWTD_init(_u8	p_u8ThreadNum, _u16 p_u16TimeOut)
{
	DBG_ASSERT(p_u8ThreadNum > 0);
	DBG_ASSERT(p_u16TimeOut > 0);
	INIT_STATIC_PARAM(g_pstThreadWTDMgr, ThreadWTD_Mgr_s, false);
	g_pstThreadWTDMgr->u8ThreadNum = p_u8ThreadNum;
	g_pstThreadWTDMgr->u16TimeOut = p_u16TimeOut;

	g_pstThreadWTDMgr->szThreadWTD = SYS_MALLOC(sizeof(ThreadWTD_s)* p_u8ThreadNum);
	DBG_ASSERT(g_pstThreadWTDMgr->szThreadWTD != NULL);

	_u32 u32CurTime = getCurSysTime_ms();
	for (_u8 i = 0; i < g_pstThreadWTDMgr->u8ThreadNum; i++)
	{
		g_pstThreadWTDMgr->szThreadWTD[i].u8ValidFlag = WTD_FLAG_INVLAID;
		g_pstThreadWTDMgr->szThreadWTD[i].u32ActiveTime = u32CurTime;
	}

	initLock(&g_pstThreadWTDMgr->stLock);

	_PRINT_LN_T("ThreadWTD_init(%u,%u)", p_u8ThreadNum, p_u16TimeOut);
	return true;
}

_VOID	ThreadWTD_uninit(_VOID)
{
	if (g_pstThreadWTDMgr == NULL){
		return;
	}
	deleteLock(&g_pstThreadWTDMgr->stLock);
	SYS_FREE(g_pstThreadWTDMgr->szThreadWTD);
	SYS_FREE(g_pstThreadWTDMgr);
}

_u8		ThreadWTD_requestWTDId(const _CHAR *p_szThreadName)
{
	if (g_pstThreadWTDMgr == NULL){
		return 0xFF;
	}
	_u32 u32CurTime = getCurSysTime_ms();
	for (_u8 u8WTDId = 0; u8WTDId < g_pstThreadWTDMgr->u8ThreadNum; u8WTDId++)
	{
		ThreadWTD_s *pstThreadWTD = g_pstThreadWTDMgr->szThreadWTD + u8WTDId;
		if (pstThreadWTD->u8ValidFlag != WTD_FLAG_INVLAID){
			continue;
		}
		pstThreadWTD->u8ValidFlag = WTD_FLAG_DISABLE;
		pstThreadWTD->u32ActiveTime = u32CurTime;
		_u32 u32Strlen = (_u32)strlen(p_szThreadName);
		u32Strlen = MIN_NUM(u32Strlen, (MAX_LEN_WTD_THREAD_NAME - 1));
		memcpy(pstThreadWTD->szThreadName, p_szThreadName, u32Strlen);
		_PRINT_LN_T("requestWTD[%s]:%d", p_szThreadName, u8WTDId);
		return u8WTDId;
	}
	DBG_ASSERT(0);
	return 0xFF;
}

_VOID	ThreadWTD_enable(_u8 p_u8WTDId)
{
	if (g_pstThreadWTDMgr == NULL){
		return;
	}
	DBG_ASSERT(p_u8WTDId < g_pstThreadWTDMgr->u8ThreadNum);
	lock(&g_pstThreadWTDMgr->stLock);
	g_pstThreadWTDMgr->szThreadWTD[p_u8WTDId].u8ValidFlag = WTD_FLAG_ENABLE;
	unlock(&g_pstThreadWTDMgr->stLock);
}

_VOID	ThreadWTD_disable(_u8 p_u8WTDId)
{
	if (g_pstThreadWTDMgr == NULL){
		return;
	}
	DBG_ASSERT(p_u8WTDId < g_pstThreadWTDMgr->u8ThreadNum);
	lock(&g_pstThreadWTDMgr->stLock);
	g_pstThreadWTDMgr->szThreadWTD[p_u8WTDId].u8ValidFlag = WTD_FLAG_DISABLE;
	unlock(&g_pstThreadWTDMgr->stLock);
	//_PRINT_LN_T("disableThread[%d,%s]", p_u8WTDId, g_pstThreadWTDMgr->szThreadWTD[p_u8WTDId].szThreadName);
}

_VOID	ThreadWTD_run(_u8 p_u8WTDId)
{
	if (g_pstThreadWTDMgr == NULL){
		return;
	}
	DBG_ASSERT(p_u8WTDId < g_pstThreadWTDMgr->u8ThreadNum);
	//lock(&g_pstThreadWTDMgr->stLock);
	g_pstThreadWTDMgr->szThreadWTD[p_u8WTDId].u32ActiveTime = getCurSysTime_ms();
	//unlock(&g_pstThreadWTDMgr->stLock);
	//_PRINT_LN_T("WTD_run:%u", p_u8WTDId);
}

_u8		ThreadWTD_isTimeout(_VOID)
{
	_u8 u8WTDId_timeout = 0xFF;
	for (_u8 u8WTDId = 0; u8WTDId < g_pstThreadWTDMgr->u8ThreadNum; u8WTDId++)
	{
		ThreadWTD_s *pstThreadWTD = g_pstThreadWTDMgr->szThreadWTD + u8WTDId;
		if (pstThreadWTD->u8ValidFlag != WTD_FLAG_ENABLE){
			continue;
		}

		//lock(&g_pstThreadWTDMgr->stLock);
		// ��ȡ��ʱ����ܱ�֤�������Ĳ�ֵ��Ϊ����,����ȡϵͳʱ�����ȡ�ʱ���п��ܳ����̵߳��ȶ�����ʱ��ʱ����º��ټ����ֵ,���²�ֵΪ����(��������)
		_u32 u32ActiveTime_last = pstThreadWTD->u32ActiveTime;		
		_u32 u32CurTime = getCurSysTime_ms();
		_u32 u32Timeout = u32CurTime - u32ActiveTime_last;
		//unlock(&g_pstThreadWTDMgr->stLock);
		if (u32Timeout > g_pstThreadWTDMgr->u16TimeOut){
			u8WTDId_timeout = u8WTDId;
			_PRINT_LN_T("Thread[%d,%s] timeout:%u ms", u8WTDId, pstThreadWTD->szThreadName, u32Timeout);
		}
	}
	return u8WTDId_timeout;
}
