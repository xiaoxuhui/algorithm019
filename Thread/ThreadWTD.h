#ifndef _THREAD_WTD_H_
#define _THREAD_WTD_H_

#include "TypeDef.h"
#include "CmdStruct.h"
#include "EntityStruct.h"

/**********************************************************************
//	�߳̿��Ź�
//	�ṩ�̻߳״̬�Ĺ���,���������»ʱ����ж��Ƿ�ʱ�Ĺ���
*********************************************************************/


//	��ʼ���߳̿��Ź�
_BOOL	ThreadWTD_init(_u8	p_u8ThreadNum, _u16 p_u16TimeOut);

//	����1��Id
//	����:	
//		p_szThreadName	�߳���
//	����ֵ��
//		WTDId
_u8		ThreadWTD_requestWTDId(const _CHAR *p_szThreadName);

// ���ÿ��Ź�
_VOID	ThreadWTD_enable(_u8 p_u8WTDId);
// ���ÿ��Ź�
_VOID	ThreadWTD_disable(_u8 p_u8WTDId);

//	�߳�����,��Ҫ��������,ͨ���÷����������̵߳Ļ״̬
_VOID	ThreadWTD_run(_u8 p_u8WTDId);

//	����Ƿ����̳߳�ʱ
//	����ֵ:
//		0xFF	��ʾ���̳߳�ʱ
//		����ֵ	��ʾ��ʱ�̵߳�WTDId
_u8		ThreadWTD_isTimeout(_VOID);

//	����ʼ��
_VOID	ThreadWTD_uninit(_VOID);
#endif
