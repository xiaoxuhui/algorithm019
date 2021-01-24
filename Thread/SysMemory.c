#include "SysMemory.h"
#include "SysLog.h"
#include "SysPrint.h"
#include "SysConfig.h"
#include "EncryKey.h"
ENCRY_KEY_2()

_u8		g_u8EncryRet = 0;

_BOOL g_bIsAssert = false;
//By Chengtian 2017-11-21 接口内部根据系统区分，malloc失败则assert
#ifdef OS_LINUX_DRIVER
#include <linux/slab.h>
#include <linux/string.h>

#define _gsg_malloc_(_MemLen_)		kmalloc(_MemLen_, GFP_KERNEL)
#define _gsg_free_(_MemPoint_)		kfree(_MemPoint_)
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef OS_RTOS
#include "GSG_Stack_Include_Adapter.h"

#ifdef CACHE_AS_RAM
#define _gsg_malloc_(_MemLen_)		ICall_malloc(_MemLen_)
#define _gsg_free_(_MemPoint_)			ICall_free(_MemPoint_)
#else
#define _gsg_malloc_(_MemLen_)		malloc(_MemLen_)
#define _gsg_free_(_MemPoint_)			free(_MemPoint_)
#endif
#else

#define _gsg_malloc_(_MemLen_)		malloc(_MemLen_)
#define _gsg_free_(_MemPoint_)			free(_MemPoint_)
#endif	// OS_RTOS
#endif	// OS_LINUX_DRIVER

#if DEBUG_MEM
#ifdef OS_RTOS
#define MAX_MEM_POS_RECORD			500
#else
#define MAX_MEM_POS_RECORD			2000
#endif
#define MAX_FILE_PATH_SIZE			32

typedef struct _MallocPos_s
{
	_BOOL bUsedFlag;
	_UINT nPoint;
	_UINT nMemSize;
	_CHAR szFile[MAX_FILE_PATH_SIZE];
	_UINT nLine;

}MallocPos_s;

typedef struct _MallocInfo_s
{
	MallocPos_s stMallocPosList[MAX_MEM_POS_RECORD];

	_UINT	nMaxMallocMem;
	_UINT	nCurMallocMem;
	_UINT	nMaxMemSize;
}MallocInfo_s;

static MallocInfo_s *g_pstMallocInfo = NULL;

_BOOL	Mem_init(const _UINT p_nMaxMemSize)
{
	g_pstMallocInfo = _gsg_malloc_(sizeof(MallocInfo_s));
	DBG_ASSERT(NULL != g_pstMallocInfo);
	memset(g_pstMallocInfo, 0, sizeof(MallocInfo_s));

	g_pstMallocInfo->nMaxMemSize = p_nMaxMemSize;
	return true;
}

_VOID	Mem_uninit(_VOID)
{
	CHECK_NULL(g_pstMallocInfo);
	_gsg_free_(g_pstMallocInfo);
}

_CHAR *getFileName(_CHAR* p_szFile)
{
	size_t u16Len = strlen(p_szFile);
	for (size_t i = 0; i < u16Len; i++)
	{
		size_t u16Offset = u16Len - 1 - i;
		if (p_szFile[u16Offset] == '/' || p_szFile[u16Offset] == '\\'){
			return p_szFile + u16Offset + 1;
		}
	}
	return p_szFile;
}

static _VOID recordMallocPos(_VOID* p_pPoint, _UINT p_nMemSize, _CHAR* p_szFile, _UINT p_nLine)
{

	for (_UINT i = 0; i < MAX_MEM_POS_RECORD; ++i)
	{
		if (!g_pstMallocInfo->stMallocPosList[i].bUsedFlag)
		{
			g_pstMallocInfo->stMallocPosList[i].nMemSize = p_nMemSize;
			g_pstMallocInfo->stMallocPosList[i].nPoint = (_UINT)p_pPoint;
			g_pstMallocInfo->stMallocPosList[i].nLine = p_nLine;

			memset(g_pstMallocInfo->stMallocPosList[i].szFile, 0, MAX_FILE_PATH_SIZE);

			_CHAR *szFileName = getFileName(p_szFile);
			_UINT nLen = (_UINT)strlen(szFileName);
			if (nLen >= MAX_FILE_PATH_SIZE)
			{
				memcpy(g_pstMallocInfo->stMallocPosList[i].szFile, szFileName + nLen - MAX_FILE_PATH_SIZE + 1, MAX_FILE_PATH_SIZE - 1);
			}
			else
			{
				strcpy(g_pstMallocInfo->stMallocPosList[i].szFile, szFileName);
			}

			g_pstMallocInfo->stMallocPosList[i].bUsedFlag = true;
			g_pstMallocInfo->nCurMallocMem += p_nMemSize;

			if (g_pstMallocInfo->nCurMallocMem > g_pstMallocInfo->nMaxMallocMem)
			{
				g_pstMallocInfo->nMaxMallocMem = g_pstMallocInfo->nCurMallocMem;
			}

			return;
		}
	}

	_PRINT_("Not enough malloc infopos buffer.\n");
}

static _VOID recordFreePos(_VOID* p_pPoint, _CHAR* p_szFile, _UINT p_nLine)
{
	for (_UINT i = 0; i < MAX_MEM_POS_RECORD; ++i)
	{
		if (g_pstMallocInfo->stMallocPosList[i].nPoint != (_UINT)p_pPoint) {
			continue;
		}

		if (!g_pstMallocInfo->stMallocPosList[i].bUsedFlag)
		{
			D_LOG("Free error! This(%p) has been free. (%s %d) \n", g_pstMallocInfo->stMallocPosList[i].nPoint, p_szFile, p_nLine);
			return;
		}

		g_pstMallocInfo->stMallocPosList[i].bUsedFlag = false;
		g_pstMallocInfo->nCurMallocMem -= g_pstMallocInfo->stMallocPosList[i].nMemSize;
		return;
	}
}

_VOID showMallocMemInfo(_VOID)
{
	_PRINT_("-----------DEBUG_MEM------------\r\n");
	//waitTime(100);
	_PRINT_("Max malloc mem = %d \r\n", g_pstMallocInfo->nMaxMallocMem);
	//waitTime(100);
	_PRINT_("Cur malloc mem = %d \r\n", g_pstMallocInfo->nCurMallocMem);

	for (_UINT i = 0; i < MAX_MEM_POS_RECORD; ++i)
	{
		if (g_pstMallocInfo->stMallocPosList[i].bUsedFlag)
		{
			//waitTime(100);
			_PRINT_("ID(%d) Mem(%x) Size=%d\t%s %d \r\n", i,
				g_pstMallocInfo->stMallocPosList[i].nPoint,
				g_pstMallocInfo->stMallocPosList[i].nMemSize,
				g_pstMallocInfo->stMallocPosList[i].szFile,
				g_pstMallocInfo->stMallocPosList[i].nLine
				);
		}
	}
	//waitTime(100);
	//_u8 *pszData[] = { 0 };
	//for (size_t i = 0; i < 64; i++)
	//{
	//	_PRINT_("alloc:%u KB \r\n",i+1);
	//	pszData[i] = SYS_MALLOC(1024);
	//	memset(pszData[i], 0, 1024);
	//}
	_PRINT_("-----------------------\r\n");
}
#else
_BOOL	Mem_init(const _UINT p_nMaxMemSize)
{
	return true;
}

_VOID Mem_uninit(_VOID)
{}

static _VOID recordMallocPos(_VOID* p_pPoint, _UINT p_nMemSize, _CHAR* p_szFile, _UINT p_nLine)
{}

static _VOID recordFreePos(_VOID* p_pPoint, _CHAR* p_szFile, _UINT p_nLine)
{}

_VOID showMallocMemInfo(_VOID)
{}
#endif // DEBUG_MEM

static _BOOL g_bPrintFlag = false;

_VOID enablePrint(_VOID)
{
	g_bPrintFlag = true;
}

_VOID* mallocSysMem(_UINT p_nMemLen, _CHAR* p_szFile, _UINT p_nLine)
{

#if DEBUG_MEM
	if (g_pstMallocInfo != NULL)
	{
		if (g_pstMallocInfo->nCurMallocMem + p_nMemLen > g_pstMallocInfo->nMaxMemSize){
			D_LOG("SysMalloc error! size=%d , MaxSize = %d ,CurSize = %d, %s %d \n", p_nMemLen, g_pstMallocInfo->nMaxMallocMem, g_pstMallocInfo->nCurMallocMem, p_szFile, p_nLine);
			return NULL;
		}
	}
#endif
	if (g_bPrintFlag){
#ifndef OS_LINUX_DRIVER
		printf("malloc(%d\t) in %s_%d\r\n", p_nMemLen, p_szFile, p_nLine);
#endif
	}
	_VOID* p = _gsg_malloc_(p_nMemLen);
	if (p == NULL){
		//printf("SysMalloc error! size=%d , %s %d \r\n", p_nMemLen, p_szFile, p_nLine);
		DBG_ASSERT(0);
	}

#if DEBUG_MEM
	recordMallocPos(p, p_nMemLen, p_szFile, p_nLine);
#endif	
	return p;
}

_VOID freeSysMem(_VOID* p_pMem, _CHAR* p_szFile, _UINT p_nLine)
{
	if (p_pMem == NULL){
		return;
	}

#if DEBUG_MEM
	recordFreePos(p_pMem, p_szFile, p_nLine);
#endif

	_gsg_free_(p_pMem);
}
