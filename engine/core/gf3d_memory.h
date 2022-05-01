#pragma once
#include "defines.hpp"

namespace gf3d
{
	enum class memory_type
	{
		eUnknown,
		eAllocator,
		eDynamic,
		eNew,
		eMaxTypes
	};

	void* malloc(const u64& size, const memory_type& type);

	void free(void* data, const u64& size, const memory_type& type);

	void log_memory_usage();
}