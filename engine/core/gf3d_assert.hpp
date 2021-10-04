#pragma once

#include  "defines.hpp"

namespace gf3d
{
	// definition in gf3d_logger.cpp
	void ReportAssertionFailure(const char* condition, const char* filename, i64 line, const char* message = "");
}

#ifndef NDEBUG
#define GFASSERT(condition, message)										\
	if(!(condition)) {														\
		ReportAssertionFailure(#condition, __FILE__, __LINE__, message);	\
		std::terminate();													\
	}
#else
#define GFASSERT(condition, message) ((void)0)
#endif // !NDEBUG
