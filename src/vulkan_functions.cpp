#include "vulkan_functions.h"

AllocatedBuffer createBuffer(const VmaAllocator& allocator, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.usage = usage;
	bufferInfo.size = allocSize;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsage;

	AllocatedBuffer newBuffer = {};

	VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &newBuffer.buffer, &newBuffer.allocation, nullptr));

	return newBuffer;
}
