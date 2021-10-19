#pragma once

#include  "defines.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#define debug_break() __debugbreak()
#elif __APPLE__
#define debug_break() __asm("int $3")
#else
#define debug_break() __builtin_trap()
#endif // _MSC_VER


namespace gf3d
{
	// definition in gf3d_logger.cpp
	void ReportAssertionFailure(const char* condition, const char* filename, i64 line, const char* message = "");
}

#ifndef NDEBUG
#define GFASSERT(condition, message)										\
	if(!(condition)) {														\
		gf3d::ReportAssertionFailure(#condition, __FILE__, __LINE__, message);	\
		debug_break();														\
	}
#else
#define GFASSERT(condition, message) ((void)0)
#endif // !NDEBUG
