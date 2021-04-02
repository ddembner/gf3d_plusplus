#pragma once
#include <vulkan/vulkan.h>
#include <cassert>
#define VK_CHECK(call)								\
	do {											\
		VkResult result = call;					\
		assert(result == VK_SUCCESS);		\
	} while(0)