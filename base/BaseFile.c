#include "BaseFile.h"
#include <stdio.h>
#include "CodeDef.h"


_INT openFile(File_s* p_pFile, const _CHAR* p_szFilePath, FileMode_e p_eMode)
{
#ifdef OS_RTOS
#else
	CHECK_NULL(p_pFile, -1);
	CHECK_NULL(p_szFilePath, -1);

	_CHAR* pMode = NULL;
	switch (p_eMode)
	{
	case READ:
		pMode = "rb";
		break;
	case READ_WRITE:
		pMode = "rb+";
		break;
	case CREATE_WRITE:
		pMode = (char*)"wb+";
		break;
	case ADD_WRITE:
		pMode = (char*)"ab+";
		break;
	default:
		return -2;
	}

	FILE* pFile = fopen(p_szFilePath, pMode);
	if (pFile == NULL) {
		return -3;
	}
	p_pFile->fd = (_VOID*)pFile;
#endif
	return 0;
}

#define BUFF_LEN 32
_UINT getFileSize(const File_s* p_pFile)
{
	CHECK_NULL(p_pFile, 0);
#ifdef OS_RTOS
	return 0;
#else
	//const _UINT nLen = 32;
	_CHAR buffer[BUFF_LEN] = { 0 };
	_UINT nReadLen = 0;
	_UINT nFileSize = 0;

	_UINT nPos = ftell((FILE*)p_pFile->fd);

	seekFile(p_pFile, 0);
	do 
	{
		if (!readFile(p_pFile, buffer, BUFF_LEN, &nReadLen))
		{
			return 0;
		}
		nFileSize += nReadLen;
		
	} while (BUFF_LEN == nReadLen);
	seekFile(p_pFile, nPos);
	return nFileSize;
#endif
}

_BOOL seekFile(const File_s* p_pFile, _UINT p_nPos)
{
	CHECK_NULL(p_pFile, false);

	//	SEEK_SET： 文件开头
	//	SEEK_CUR： 当前位置
	//	SEEK_END： 文件结尾
#ifdef OS_RTOS
#else
	if (fseek((FILE*)p_pFile->fd, p_nPos, SEEK_SET) != 0)
	{
		return false;
	}
#endif
	return true;
}

_BOOL readFile(const File_s* p_pFile, _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_outSize)
{
	CHECK_NULL(p_pFile, false);
	CHECK_NULL(p_pBuffer, false);
	CHECK_NULL(p_outSize, false);
#ifdef OS_RTOS
#else
	_UINT nRet = fread(p_pBuffer, 1, p_nBufferSize, (FILE*)p_pFile->fd);
	if (nRet > p_nBufferSize)
	{
		*p_outSize = 0;
		return false;
	}

	*p_outSize = nRet;
#endif
	return true;
}

_BOOL writeFile(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nSize)
{
	CHECK_NULL(p_pFile, false);
	CHECK_NULL(p_pBuffer, false);
#ifdef OS_RTOS
#else
	_UINT nNeedWriteSize = p_nSize;
	_UINT nWriteZeroCount = 0;	//连续3次写入为0

	while (nNeedWriteSize != 0)
	{
		_UINT nRet = fwrite(p_pBuffer, 1, nNeedWriteSize, (FILE*)p_pFile->fd);
		if (nRet == 0)
		{
			nWriteZeroCount++;
		}
		else
		{
			nWriteZeroCount = 0;
		}

		if ((nWriteZeroCount >= 3) && (nNeedWriteSize!=0))//在还未写完的情况下  连续3次写入为0则失败
		{
			return false;
		}
		nNeedWriteSize -= nRet;
	}
#endif
	return true;
}

_BOOL writeFile2(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_nHasWriteLen)
{
	CHECK_NULL(p_pFile, false);
	CHECK_NULL(p_pBuffer, false);
	CHECK_NULL(p_nHasWriteLen, false);
#ifdef OS_RTOS
#else
	*p_nHasWriteLen = fwrite(p_pBuffer, 1, p_nBufferSize, (FILE*)p_pFile->fd);
#endif
	return true;
}

_VOID flushFile(const File_s* p_pFile)
{
	CHECK_NULL(p_pFile);
#ifdef OS_RTOS
#else
	fflush((FILE*)p_pFile->fd);
#endif
}

_VOID closeFile(File_s* p_pFile)
{
	CHECK_NULL(p_pFile);
#ifdef OS_RTOS
#else
	fclose((FILE*)p_pFile->fd);
	p_pFile->fd = NULL;
#endif
	return;
}


_BOOL File_readAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _UINT p_nBufferSize, _u32 *p_pu32DataLen)
{// 一次性读取文件中所有数据内容
	//读文件
	File_s stFile = { 0 };
	_INT nRet = openFile(&stFile, p_szFileName, READ);
	if (nRet != 0){
		return false;
	}

	_u32 u32FileLen = getFileSize(&stFile);
	if (u32FileLen >= p_nBufferSize){
		closeFile(&stFile);
		return false;
	}

	_u32 nFileSize = 0;
	_BOOL bRet = readFile(&stFile, p_szData, p_nBufferSize, &nFileSize);
	closeFile(&stFile);
	if (!bRet){
		return false;
	}
	*p_pu32DataLen = nFileSize;
	return true;
}

_BOOL File_writeAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _u32 p_u32DataLen)
{// 一次性将所有数据写入文件中

	File_s stFile = { 0 };
	_INT nRet = openFile(&stFile, p_szFileName, CREATE_WRITE);
	if (nRet != 0){
		return false;
	}

	_BOOL bRet = writeFile(&stFile, p_szData, p_u32DataLen);
	closeFile(&stFile);
	if (!bRet){
		return false;
	}
	return true;
}
