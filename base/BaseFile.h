#ifndef _BASE_FILE_H_
#define _BASE_FILE_H_

#include "TypeDef.h"

typedef struct _File_s
{
	_VOID*	fd;
}File_s;

typedef	enum _FileMode_e
{
	READ = 1,			//只读
	READ_WRITE = 2,		//读写
	CREATE_WRITE = 3,	//覆盖读写（若文件不存在，则会建立该文件）
	ADD_WRITE = 4		//追加读写（若文件不存在，则会建立该文件）
}FileMode_e;


//成功返回 0
//失败返回 -1：之前文件未关闭	-2：打开模式错误	-3：打开失败
_INT openFile(File_s* p_pFile, const _CHAR* p_szFilePath, FileMode_e p_eMode);

//如未成功打开文件  则返回0
_UINT getFileSize(const File_s* p_pFile);

//文件读写位置定位	从文件头开始（有需要再添加其他位置）
_BOOL seekFile(const File_s* p_pFile, _UINT p_nPos);

//读文件
//p_nBufferSize：想要读的字节数
//p_outSize：实际读到的字节数（失败时为0）
_BOOL readFile(const File_s* p_pFile, _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_outSize);

//写文件
//内有循环  帮助写入完整	在还未写完的情况下  连续3次写入为0则返回false失败
_BOOL writeFile(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nBufferSize);
//不保证数据写入完整	用户需要自己根据p_nHasWriteLen判断写入的数据长度
_BOOL writeFile2(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_nHasWriteLen);

//用户空间缓冲区同步到文件中（针对写文件时）
_VOID flushFile(const File_s* p_pFile);

_VOID closeFile(File_s* p_pFile);

// 一次性读取文件中所有数据内容
_BOOL File_readAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _UINT p_nBufferSize, _u32 *p_pu32DataLen);

// 一次性将所有数据写入文件中
_BOOL File_writeAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _u32 p_u32DataLen);

#endif	//_BASE_FILE_H_

