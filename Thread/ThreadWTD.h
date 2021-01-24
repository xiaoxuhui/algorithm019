#ifndef _THREAD_WTD_H_
#define _THREAD_WTD_H_

#include "TypeDef.h"
#include "CmdStruct.h"
#include "EntityStruct.h"

/**********************************************************************
//	线程看门狗
//	提供线程活动状态的管理,包括：更新活动时间和判断是否超时的功能
*********************************************************************/


//	初始化线程看门狗
_BOOL	ThreadWTD_init(_u8	p_u8ThreadNum, _u16 p_u16TimeOut);

//	申请1个Id
//	输入:	
//		p_szThreadName	线程名
//	返回值：
//		WTDId
_u8		ThreadWTD_requestWTDId(const _CHAR *p_szThreadName);

// 启用看门狗
_VOID	ThreadWTD_enable(_u8 p_u8WTDId);
// 禁用看门狗
_VOID	ThreadWTD_disable(_u8 p_u8WTDId);

//	线程运行,需要反复调用,通过该方法来更新线程的活动状态
_VOID	ThreadWTD_run(_u8 p_u8WTDId);

//	检测是否有线程超时
//	返回值:
//		0xFF	表示无线程超时
//		其它值	表示超时线程的WTDId
_u8		ThreadWTD_isTimeout(_VOID);

//	反初始化
_VOID	ThreadWTD_uninit(_VOID);
#endif
