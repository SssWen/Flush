#pragma once
#include "flushpch.h"
// ��ʱ������ƽ̨����, ���ʹ��dll,Ԥ����FLUSH_BUILD_DLL
#ifdef FLUSH_PLATFORM_WINDOW
	#ifdef FLUSH_BUILD_DLL 
		#define FLUSH_API __declspec(dllexport)
	#else
		#define FLUSH_API __declspec(dllimport)
	#endif
#else
	// ʹ��staticlib,
	#define FLUSH_API 
#endif 
// HZ_PLATFORM_WINDOW

#define BIT(x) (1 << x)

#define FLUSH_BIND_EVENT_FN(fn) std::bind(&fn, this,std::placeholders::_1)
