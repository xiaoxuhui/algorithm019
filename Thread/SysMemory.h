#ifndef _SYS_MEMORY_H_
#define _SYS_MEMORY_H_

#include "TypeDef.h"
#include "CodeDef.h"
#include "SysConfig.h"
#include "SysPrint.h"

_BOOL	Mem_init(const _UINT p_nMaxMemSize);

_VOID	Mem_uninit(_VOID);

_VOID* mallocSysMem(_UINT p_nMemLen, _CHAR* p_szFile, _UINT p_nLine);

_VOID freeSysMem(_VOID* p_pMem, _CHAR* p_szFile, _UINT p_nLine);

//显示内存使用信息
_VOID showMallocMemInfo(_VOID);

_VOID enablePrint(_VOID);

//By Chengtian 2017-11-21 不再根据系统定义不同的malloc和free宏，统一调用接口
#define SYS_MEM_INIT(maxMemSize) Mem_init(maxMemSize)
#define SYS_MEM_UNINIT() Mem_uninit()
#define SHOW_MEM_INFO() showMallocMemInfo()

#define SYS_MALLOC(_MemLen_) mallocSysMem(_MemLen_, __FILE__, __LINE__)

#define SYS_FREE(_MemPoint_) do \
{\
if (0 == (unsigned int)_MemPoint_) { break; }\
	freeSysMem(_MemPoint_, __FILE__, __LINE__); \
	_MemPoint_ = 0; \
} while (0)

#endif	//_SYS_MEMORY_H_
