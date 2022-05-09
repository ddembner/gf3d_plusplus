#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "defines.hpp"
#include "gf3d_device.h"

struct VulkanImage
{
	VkImage image = nullptr;
	VmaAllocation allocation = nullptr;
	VkImageView view = nullptr;
	u32 width = 0;
	u32 height = 0;

	VulkanImage() = default;
	
	VulkanImage(
		Gf3dDevice* device,
		VkImageType imageType,
		u32 width,
		u32 height,
		VkFormat format,
		VkImageUsageFlags usage,
		VkImageTiling tiling,
		VmaMemoryUsage memoryUsage
	);

	void createImageView(Gf3dDevice* device, VkFormat format, VkImageAspectFlags aspectFlags);

	void destroy(Gf3dDevice* device);

	void transitionLayout(
		Gf3dDevice* device,
		VkCommandBuffer commandBuffer,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout
	);
};