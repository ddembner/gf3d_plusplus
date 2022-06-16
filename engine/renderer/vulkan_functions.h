#pragma once
#include "gf3d_device.h"
#include <cassert>
#include "vulkan_types.h"
#define VK_CHECK(call)								\
	do {											\
		VkResult result = call;					\
		assert(result == VK_SUCCESS);		\
	} while(0)

VulkanBuffer createAllocatedBuffer(
	VmaAllocator allocator, 
	size_t allocSize, 
	VkBufferUsageFlags usage, 
	VmaMemoryUsage memoryUsage
);

void vulkanCommandBufferSingleUseBegin(Gf3dDevice* device, VkCommandPool pool, VkCommandBuffer* cmd);

void vulkanCommandBufferSingleUseEnd(Gf3dDevice* device, VkCommandPool pool, VkCommandBuffer* cmd, VkQueue queue);