#include <iostream>
#include "gf3d_swapchain.h"
#include "vulkan_functions.h"

Swapchain::Swapchain()
{
}

Swapchain::~Swapchain()
{
}

void Swapchain::init(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	extent = surfaceCapabilities.currentExtent;

	selectPresentMode(physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat = getSurfaceFormat(physicalDevice, surface);
	colorFormat = surfaceFormat.format;

	VkCompositeAlphaFlagBitsKHR compositeAlpha = selectCompositeAlphaFlags(surfaceCapabilities);

	VkSurfaceTransformFlagBitsKHR preTransform = selectSurfaceTransformFlags(surfaceCapabilities);

	uint32_t numImages = getNumberOfImagesForBuffers(surfaceCapabilities);

	VkSwapchainCreateInfoKHR swapchainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapchainInfo.surface = surface;
	swapchainInfo.oldSwapchain = oldSwapchain;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.compositeAlpha = compositeAlpha;
	swapchainInfo.preTransform = preTransform;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.minImageCount = numImages;
	swapchainInfo.imageFormat = surfaceFormat.format;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain) != VK_SUCCESS) {
		std::cout << "Failed to create swapchain" << std::endl;
	}

	if (oldSwapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
	}

	//keep the old swapchain for when a new one has to be remade
	oldSwapchain = swapchain;

	vkGetSwapchainImagesKHR(device, swapchain, &bufferImageCount, nullptr);
	std::vector<VkImage> images(bufferImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &bufferImageCount, images.data());

	buffers.resize(bufferImageCount);
	for (uint32_t i = 0; i < bufferImageCount; i++) {
		buffers[i].image = images[i];

		VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.image = buffers[i].image;
		viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		viewInfo.format = surfaceFormat.format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;

		if (vkCreateImageView(device, &viewInfo, nullptr, &buffers[i].view) != VK_SUCCESS) {
			std::cout << "Failed to create image view for framebuffer" << std::endl;
			return;
		}
	}

	createRenderPass(device);

	createFrameBuffers(device);
}

void Swapchain::cleanup(VkDevice device)
{
	for (int i = 0; i < frameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	}

	for (int i = 0; i < buffers.size(); i++) {
		vkDestroyImageView(device, buffers[i].view, nullptr);
	}

	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}

uint32_t Swapchain::imageCount() const
{
	return bufferImageCount;
}

VkFormat Swapchain::getColorFormat() const
{
	return colorFormat;
}

VkExtent2D Swapchain::getExtent() const
{
	return extent;
}

VkSwapchainKHR Swapchain::getSwapchain() const
{
	return swapchain;
}

VkRenderPass Swapchain::getRenderPass() const
{
	return renderPass;
}

VkFramebuffer Swapchain::getFrameBuffer(uint32_t index)
{
	return frameBuffers[index];
}

void Swapchain::recreate(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
{
	for (int i = 0; i < frameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	}

	for (int i = 0; i < buffers.size(); i++) {
		vkDestroyImageView(device, buffers[i].view, nullptr);
	}

	vkDestroyRenderPass(device, renderPass, nullptr);

	init(physicalDevice, device, surface);
}

void Swapchain::selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	uint32_t presentCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, presentModes.data());

	//This is the only mode to be guaranteed under the standard
	presentMode = VK_PRESENT_MODE_FIFO_KHR;

	for (auto& mode : presentModes) {
		//prefer to use mailbox if available
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = mode;
			break;
		}
	}

}

VkCompositeAlphaFlagBitsKHR Swapchain::selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
	VkCompositeAlphaFlagBitsKHR alphaFlag = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	std::vector< VkCompositeAlphaFlagBitsKHR> alphaFlagBits = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
	};

	for (const auto& flag : alphaFlagBits) {
		if (surfaceCapabilities.supportedCompositeAlpha & flag) {
			return flag;
		}
	}

	return alphaFlag;
}

VkSurfaceTransformFlagBitsKHR Swapchain::selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}

	return surfaceCapabilities.currentTransform;
}

uint32_t Swapchain::getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
	//2 is the standard minimum and adding 1 for triple buffering
	uint32_t numberOfImages = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && numberOfImages > surfaceCapabilities.maxImageCount) {
		numberOfImages = surfaceCapabilities.maxImageCount;
	}

	return numberOfImages;
}

VkSurfaceFormatKHR Swapchain::getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

	for (auto& surfaceFormat : surfaceFormats) {

		if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
			return surfaceFormat;
		}
	}

	return surfaceFormats[0];
}

void Swapchain::createRenderPass(VkDevice device)
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = colorFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependacy = {};
	dependacy.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependacy.dstSubpass = 0;
	dependacy.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.srcAccessMask = 0;
	dependacy.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &colorAttachment;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpassDescription;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependacy;

	VK_CHECK(vkCreateRenderPass(device, &createInfo, nullptr, &renderPass));
}

void Swapchain::createFrameBuffers(VkDevice device)
{
	frameBuffers.resize(bufferImageCount);
	VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.layers = 1;
	for (size_t i = 0; i < frameBuffers.size(); i++) {
		framebufferInfo.pAttachments = &buffers[i].view;
		VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &frameBuffers[i]));
	}
}