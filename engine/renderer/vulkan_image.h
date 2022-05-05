#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "defines.hpp"
#include "gf3d_device.h"

struct VulkanImage
{
	VkImage image;
	VmaAllocation allocation;
	VkImageView view;

	VulkanImage() = default;
	
	VulkanImage(
		Gf3dDevice* _gf3dDevice,
		VkImageType _imageType,
		u32 _width,
		u32 _height,
		VkFormat _format,
		VkImageUsageFlags _usage,
		VkImageTiling _tiling,
		VmaMemoryUsage _memoryUsage
	);

	void destroy(Gf3dDevice* _gf3dDevice);
};