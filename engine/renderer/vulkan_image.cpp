#include "core/gf3d_logger.h"
#include "vulkan_image.h"
#include "vulkan_functions.h"


VulkanImage::VulkanImage( 
	Gf3dDevice* device,
	VkImageType imageType,
	u32 width,
	u32 height,
	VkFormat format,
	VkImageUsageFlags usage,
	VkImageTiling tiling,
	VmaMemoryUsage memoryUsage)
{
	this->width = width;
	this->height = height;

	VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageInfo.imageType = imageType;
	imageInfo.extent = { width, height, 1 };
	imageInfo.format = format;
	imageInfo.arrayLayers = 1;
	imageInfo.mipLevels = 1;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.usage = usage;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsage;

	VK_CHECK(vmaCreateImage(device->GetAllocator(), &imageInfo, &allocInfo, &image, &allocation, nullptr));
}

void VulkanImage::createImageView(Gf3dDevice* device, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	viewInfo.format = format;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.subresourceRange.aspectMask = aspectFlags;

	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.layerCount = 1;
	viewInfo.subresourceRange.levelCount = 1;

	VK_CHECK(vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &view));
}

void VulkanImage::destroy(Gf3dDevice* device)
{
	if (view) {
		vkDestroyImageView(device->GetDevice(), view, nullptr);
		view = nullptr;
	}
	if (image) {
		vmaDestroyImage(device->GetAllocator(), image, allocation);
		image = nullptr;
		allocation = nullptr;
	}
	width = 0;
	height = 0;
}

void VulkanImage::transitionLayout(
	Gf3dDevice* device, 
	VkCommandBuffer commandBuffer, 
	VkFormat format, 
	VkImageLayout oldLayout, 
	VkImageLayout newLayout)
{
	VkImageMemoryBarrier imageBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	imageBarrier.oldLayout = oldLayout;
	imageBarrier.newLayout = newLayout;
	imageBarrier.image = image;
	imageBarrier.srcQueueFamilyIndex = device->GetGraphicsQueueIndex();
	imageBarrier.dstQueueFamilyIndex = device->GetGraphicsQueueIndex();
	imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageBarrier.subresourceRange.baseArrayLayer = 0;
	imageBarrier.subresourceRange.baseMipLevel = 0;
	imageBarrier.subresourceRange.layerCount = 1;
	imageBarrier.subresourceRange.levelCount = 1;

	VkPipelineStageFlags srcStage;
	VkPipelineStageFlags dstStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		// Transition to optimal layout
		imageBarrier.srcAccessMask = VK_ACCESS_NONE_KHR;
		imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		// Transition from transfer layout to shader read layout
		imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		LOGGER_ERROR("Unsupported image transition layout");
		return;
	}

	vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, 0, 0, 0, 1, &imageBarrier);
}

void VulkanImage::copyImageFromBuffer(Gf3dDevice* gf3dDevice, VkBuffer buffer, VkCommandBuffer cmd)
{
	VkBufferImageCopy region = {};
	region.imageExtent = { width, height, 1 };
	region.bufferImageHeight = 0;
	region.bufferRowLength = 0;
	region.bufferOffset = 0;
	
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	
	vkCmdCopyBufferToImage(cmd, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}
