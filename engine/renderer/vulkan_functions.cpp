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

void vulkanCommandBufferSingleUseBegin(Gf3dDevice* device, VkCommandPool pool, VkCommandBuffer* cmd)
{
	// Allocate command buffer
	VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandBufferCount = 1;
	allocInfo.commandPool = pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	VK_CHECK(vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, cmd));

	//Begin the command buffer
	VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VK_CHECK(vkBeginCommandBuffer(*cmd, &beginInfo));
}

void vulkanCommandBufferSingleUseEnd(Gf3dDevice* device, VkCommandPool pool, VkCommandBuffer* cmd, VkQueue queue)
{
	VK_CHECK(vkEndCommandBuffer(*cmd));

	VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = cmd;
	VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, nullptr));

	VK_CHECK(vkQueueWaitIdle(queue));

	vkFreeCommandBuffers(device->GetDevice(), pool, 1, cmd);
}
