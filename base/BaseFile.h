#ifndef _BASE_FILE_H_
#define _BASE_FILE_H_

#include "TypeDef.h"

typedef struct _File_s
{
	_VOID*	fd;
}File_s;

typedef	enum _FileMode_e
{
	READ = 1,			//ֻ��
	READ_WRITE = 2,		//��д
	CREATE_WRITE = 3,	//���Ƕ�д�����ļ������ڣ���Ὠ�����ļ���
	ADD_WRITE = 4		//׷�Ӷ�д�����ļ������ڣ���Ὠ�����ļ���
}FileMode_e;


//�ɹ����� 0
//ʧ�ܷ��� -1��֮ǰ�ļ�δ�ر�	-2����ģʽ����	-3����ʧ��
_INT openFile(File_s* p_pFile, const _CHAR* p_szFilePath, FileMode_e p_eMode);

//��δ�ɹ����ļ�  �򷵻�0
_UINT getFileSize(const File_s* p_pFile);

//�ļ���дλ�ö�λ	���ļ�ͷ��ʼ������Ҫ���������λ�ã�
_BOOL seekFile(const File_s* p_pFile, _UINT p_nPos);

//���ļ�
//p_nBufferSize����Ҫ�����ֽ���
//p_outSize��ʵ�ʶ������ֽ�����ʧ��ʱΪ0��
_BOOL readFile(const File_s* p_pFile, _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_outSize);

//д�ļ�
//����ѭ��  ����д������	�ڻ�δд��������  ����3��д��Ϊ0�򷵻�falseʧ��
_BOOL writeFile(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nBufferSize);
//����֤����д������	�û���Ҫ�Լ�����p_nHasWriteLen�ж�д������ݳ���
_BOOL writeFile2(const File_s* p_pFile, const _CHAR* p_pBuffer, _UINT p_nBufferSize, _UINT* p_nHasWriteLen);

//�û��ռ仺����ͬ�����ļ��У����д�ļ�ʱ��
_VOID flushFile(const File_s* p_pFile);

_VOID closeFile(File_s* p_pFile);

// һ���Զ�ȡ�ļ���������������
_BOOL File_readAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _UINT p_nBufferSize, _u32 *p_pu32DataLen);

// һ���Խ���������д���ļ���
_BOOL File_writeAllData(const _CHAR *p_szFileName, _CHAR *p_szData, _u32 p_u32DataLen);

#endif	//_BASE_FILE_H_

