#include "vulkan_image.h"
#include "vulkan_functions.h"

VulkanImage::VulkanImage(VmaAllocator _allocator, 
	VkImageType _imageType, 
	u32 _width, 
	u32 _height, 
	VkFormat _format, 
	VkImageUsageFlags _usage, 
	VkImageTiling _tiling, 
	VmaMemoryUsage _memoryUsage)
	: allocator(_allocator)
{
	VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageInfo.imageType = _imageType;
	imageInfo.extent = { _width, _height, 1 };
	imageInfo.format = _format;
	imageInfo.arrayLayers = 1;
	imageInfo.mipLevels = 1;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.usage = _usage;
	imageInfo.tiling = _tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = _memoryUsage;

	VK_CHECK(vmaCreateImage(allocator, &imageInfo, &allocInfo, &image, &allocation, nullptr));
}
