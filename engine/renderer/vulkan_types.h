#pragma once
#include <vk_mem_alloc.h>

struct VulkanBuffer
{
	VkBuffer buffer;
	VmaAllocation allocation;
};

struct VulkanTextureData
{
	VkImage image;
	VkSampler sampler;
};