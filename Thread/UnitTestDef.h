
#ifndef _UNIT_TEST_DEF_H_
#define _UNIT_TEST_DEF_H_

#include <string.h> //strrchr()函数所需头文件
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif 

#include "TypeDef.h"
#include "GSG.h"

//整型(_u8/s8/u16/s16/u32/s32)、BOOL均可使用该方法
#define ASSERT_V(actual,expected,msg) do \
{\
if (expected != actual) { printf("Test(%s) failed. expected(%d)/actual(%d) in fun(%s) %s line:%d \n", msg, (int)expected, (int)actual, __FUNCTION__, filename(__FILE__), __LINE__); return; }\
} while (0)

#define ASSERT_EQUAL(actual,expected) do \
{\
if ((expected) != (actual)) { printf("expected(%d)/actual(%d) in fun(%s) %s line:%d \n",(int)expected, (int)actual, __FUNCTION__, filename(__FILE__), __LINE__); return; }\
} while (0)

#define ASSERT_NOT_EQUAL(actual,expected) do \
{\
if ((expected) == (actual)) { printf("expected not(%d)/actual(%d) in fun(%s) %s line:%d \n", (int)expected, (int)actual, __FUNCTION__, filename(__FILE__), __LINE__); return; }\
} while (0)

#define ASSERT_EQUAL_RET(_value1_, _value2_, ...) do \
{\
if (_value1_ != _value2_) { printf("expected(%d)/actual(%d) in fun(%s) %s line:%d \n", (int)_value2_, (int)_value1_, __FUNCTION__, filename(__FILE__), __LINE__); return __VA_ARGS__; }\
} while (0)



#define ASSERT_TRUE(actual) do \
{\
if (!(actual)) { printf("actual(false) in fun(%s) %s line:%d \n", __FUNCTION__, filename(__FILE__), __LINE__); return; }\
} while (0)

#define ASSERT_SUCCESS(msg,...) do \
{\
	printf(msg,__VA_ARGS__);\
	printf(" in fun(%s) %s line:%d \n", __FUNCTION__, filename(__FILE__), __LINE__);\
}while (0)


#define MATCH_FAILED 0
#define MATCH_SUCCEES 1
#define FILE_NOT_EXIST 2

static _CHAR *g_szTestType[] = { "FAIL", "SUCCESS", "NO FILE" };
typedef struct _TestResultInfo_s
{
	_u16 u16TestCount;
	_u16 szTestIdList[200];
}TestResultInfo_s;

inline _VOID addTestResult(TestResultInfo_s stResultList[3], _INT p_nResult, _INT p_nTestId)
{
	stResultList[p_nResult].szTestIdList[stResultList[p_nResult].u16TestCount] = p_nTestId;
	stResultList[p_nResult].u16TestCount++;
}

inline _VOID printTestResult(TestResultInfo_s stResultList[3])
{
	for (int i = 0; i < 3; i++)
	{
		TestResultInfo_s *pstResultInfo = stResultList + i;
		_PRINT_("%s\t[%3d]:\t", g_szTestType[i], pstResultInfo->u16TestCount);
		for (size_t j = 0; j < pstResultInfo->u16TestCount; j++)
		{
			_PRINT_("%u\t", pstResultInfo->szTestIdList[j]);
		}
		_PRINT_LN("");
	}
}


#endif //_UNIT_TEST_DEF_H_
