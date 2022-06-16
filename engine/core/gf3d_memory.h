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
		eVector,
		eTexture,
		eMaxTypes
	};

	void initDynamicMemorySystem(u64 size);

	void shutdownDynamicMemorySystem();

	void* malloc(const u64& size, const memory_type& type = memory_type::eUnknown);

	void free(void* data, const u64& size, const memory_type& type);

	void log_memory_usage();
}