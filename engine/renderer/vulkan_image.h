#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "defines.hpp"

struct VulkanImage
{
	VmaAllocator allocator;
	VkImage image;
	VmaAllocation allocation;
	VkImageView view;

	VulkanImage() = default;
	
	VulkanImage(
		VmaAllocator allocator,
		VkImageType imageType,
		u32 width,
		u32 height,
		VkFormat format,
		VkImageUsageFlags usage,
		VkImageTiling tiling,
		VmaMemoryUsage memoryUsage
	);
};