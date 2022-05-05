#include "vulkan_functions.h"
#include "defines.hpp"

VulkanBuffer createAllocatedBuffer(
	VmaAllocator allocator, 
	size_t allocSize, 
	VkBufferUsageFlags 
	usage, 
	VmaMemoryUsage memoryUsage)
{
	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.usage = usage;
	bufferInfo.size = allocSize;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsage;

	VulkanBuffer newBuffer = {};

	VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &newBuffer.buffer, &newBuffer.allocation, nullptr));

	return newBuffer;
}