#pragma once
#include <vk_mem_alloc.h>
#include <glm/mat4x4.hpp>
struct VulkanBuffer
{
	VkBuffer buffer;
	VmaAllocation allocation;
};