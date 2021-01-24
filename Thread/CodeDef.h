#ifndef _CODE_DEF_H_
#define _CODE_DEF_H_

#ifndef OS_LINUX_DRIVER
#include <assert.h>
#endif
#include "TypeDef.h"
#include "SysPrint.h"
#include "SysTime.h"
#include "SysMemory.h"

#define CHECK_NULL(point, ...) do \
{\
if (NULL == point) { return __VA_ARGS__; }\
} while (0)

#define CHECK_NULL_PRINT(point, ...) do \
{\
if (NULL == point) { _PRINT_("NULL %s,%d\n", __FILE__, __LINE__); return __VA_ARGS__; }\
} while (0)

#define CHECK_ZERO(value, ...) do \
{\
if (0 == value) { return __VA_ARGS__; }\
} while (0)

#define CHECK_ET_VALUE(_value1_, _value2_, ...) do \
{\
if (_value1_ != _value2_) { return __VA_ARGS__; }\
} while (0)


#define CHECK_ET_VALUE_PRINT(_value1_, _value2_, ...) do \
{\
if (_value1_ != _value2_) { _PRINT_("N0 ET %s,%d\n", __FILE__, __LINE__); return __VA_ARGS__; }\
} while (0)

// 检测后打印错误值或者成功值
#define CHECK_ET_VALUE_PRINT_T(msg, _value1_, _value2_, ...) do \
{\
if (_value1_ != _value2_) { _PRINT_LN_T("%s failed,ret:%d", msg, _value1_); return __VA_ARGS__; }\
	else{ _PRINT_LN_T("%s OK", msg); }\
} while (0)

// 检测后只打印错误值
#define CHECK_ET_VALUE_PRINT_E(msg, _value1_, _value2_, ...) do \
{\
if (_value1_ != _value2_) { _PRINT_LN_T("%s failed,ret:%d", msg, _value1_); return __VA_ARGS__; }\
} while (0)


#define CHECK_BIG_ET_VALUE(_value1_, _value2_, ...) do \
{\
if (_value1_ < _value2_) { return __VA_ARGS__; }\
} while (0)

#define CHECK_SMALL_ET_VALUE(_value1_, _value2_, ...) do \
{\
if (_value1_ > _value2_) { return __VA_ARGS__; }\
} while (0)

#define CHECK_ET_VALUE_GOTO(_value1_, _value2_, _lable_) do \
{\
if (_value1_ != _value2_) { goto _lable_; }\
} while (0)

#define CHECK_ET_VALUE_BREAK(_value1_, _value2_) \
{\
if (_value1_ != _value2_) { break; }\
}

#define CHECK_ET_VALUE_CONTINUE(_value1_, _value2_) \
{\
if (_value1_ != _value2_) { continue; }\
}

#define SAFE_FREE(point) do \
{\
	if (NULL == point) { break; }\
	free(point);\
	point = NULL;\
} while (0)

#define  INIT_STATIC_PARAM(g_pstParam,Type,errValue) do\
{\
	SYS_FREE(g_pstParam); \
	g_pstParam = (Type*)SYS_MALLOC(sizeof(Type)); \
	CHECK_NULL(g_pstParam, errValue); \
	memset(g_pstParam, 0, sizeof(Type)); \
}while (0)

//TODO: 待封装行号打印测试
extern _BOOL g_bIsAssert;
#ifdef OS_LINUX_DRIVER
#define DBG_ASSERT(a)		do \
{ \
	if (!(a))	{\
	_PRINT_("Assert %s,%d\n", __FILE__, __LINE__); \
	g_bIsAssert = true; \
	waitTime(10000); \
	}\
	BUG_ON(!(a)); \
} while (0)
#elif defined OS_NRF
#define DBG_ASSERT(a)		do \
{ \
	if (!(a))	{\
		_PRINT_("Assert %s,%d\r\n", __FILE__, __LINE__);\
		g_bIsAssert = true;\
	}\
} while (0)
#else
#define DBG_ASSERT(a)		do \
{ \
	if (!(a))	{\
		_PRINT_("Assert %s,%d\r\n", __FILE__, __LINE__);\
		g_bIsAssert = true; \
		assert(0);\
	}\
} while (0)
#endif

#ifndef WIN32
#ifndef UINT_MAX
#define UINT_MAX	0xFFFFFFFF
#endif
#endif // !WIN32

#define MIN_NUM(a,b) ((a) < (b) ? (a): (b))
#define MAX_NUM(a,b) ((a) > (b) ? (a): (b))

//最大值限制
#define MAX_LIMIT(a,maxValue) DBG_ASSERT(maxValue>=0); if(a > maxValue){a = maxValue;}else if(a < -maxValue){a = -maxValue;}
//最小值限制
#define MIN_LIMIT(a,minValue) DBG_ASSERT(minValue>=0); if(a > 0 && a < minValue){ a = minValue;}else if(a < 0 && a > -minValue){a = -minValue;}


#ifdef OS_LINUX_DRIVER		//64位除法和取余运算
#include <linux/math64.h>

#define REM_64_DIV(a, b)			(do_div((a), (b)))
#define u64_DIV(a, b)					(div_u64((a), (b)))
#define s64_DIV(a, b)					(div_s64((a), (b)))
#define u64_DIV_64(a, b)			(div64_u64((a), (b)))
#define s64_DIV_64(a, b)			(div64_s64((a), (b)))
#else
#define REM_64_DIV(a, b)			((a) % (b))
#define u64_DIV(a, b)					((a) / (b))
#define s64_DIV(a, b)					((a) / (b))
#define u64_DIV_64(a, b)			((a) / (b))
#define s64_DIV_64(a, b)			((a) / (b))
#endif

#define ALIGN_DOWN(a, size) (a & (~(size-1)) )
#define ALIGN_DOWN_BY(a,b)	(a / b* b)
#define ALIGN_UP(a, size)   ((a+size-1) & (~ (size-1)))
#define ALIGN_UP_BY(a,b)		((a + b - 1) / b * b)
#define ALIGN_BACK(x, a)              ((a) * (((x) / (a))))
#define HALF(a)				(a >> 1)	// (a/2) 仅限无符号数据类型
#define CHECK_ADJUST_VALUE(a, minValue, maxValue) (a < minValue ?minValue:(a>maxValue?maxValue:a))


#define Rate_1000(a, b) ((b) == 0 ? 0 : ((a) * 1000 + (b/2) ) / (b))		//	计算a/b的千分比,并进行四舍五入
#define Rate_100(a, b) ((b) == 0 ? 0 : ((a) * 100 + (b/2)) / (b))				//	计算a/b的百分比,并进行四舍五入

#define FLOAT2S32(x)     (_s32)((x)+0.5)
#define FLOAT2S16(x)     (_s16)((x)+0.5)

#if  (!defined IDE_IAR && !defined OS_LINUX_DRIVER)
#define inline __inline
#endif
#endif	//_CODE_DEF_H_

