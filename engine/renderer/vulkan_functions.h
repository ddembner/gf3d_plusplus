#pragma once
#include <vulkan/vulkan.h>
#include <cassert>
#include "vulkan_types.h"
#define VK_CHECK(call)								\
	do {											\
		VkResult result = call;					\
		assert(result == VK_SUCCESS);		\
	} while(0)

AllocatedBuffer createBuffer(const VmaAllocator& allocator, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);