#include "core/gf3d_logger.h"
#include "gf3d_swapchain.h"
#include "vulkan_functions.h"
#include "gf3d_device.h"

void Swapchain::init(Gf3dDevice* device)
{
	assert(device);
	gf3dDevice = device;
	createSwapchain();
	createSwapchainImages();
	createRenderPass();
	createDepthResources();
	createFrameBuffers();
}

void Swapchain::cleanup()
{
	VkDevice device = gf3dDevice->GetDevice();
	VmaAllocator allocator = gf3dDevice->GetAllocator();

	for (int i = 0; i < frameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	}

	for (int i = 0; i < swapchainImageViews.size(); i++) {
		vkDestroyImageView(device, swapchainImageViews[i], nullptr);
	}

	vkDestroyImageView(device, depthAllocatedImage.view, nullptr);

	vmaDestroyImage(allocator, depthAllocatedImage.image, depthAllocatedImage.allocation);

	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void Swapchain::recreate()
{
	VkDevice device = gf3dDevice->GetDevice();
	VmaAllocator allocator = gf3dDevice->GetAllocator();

	for (int i = 0; i < frameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	}

	for (int i = 0; i < swapchainImageViews.size(); i++) {
		vkDestroyImageView(device, swapchainImageViews[i], nullptr);
	}

	depthAllocatedImage.destroy(gf3dDevice);

	//vkDestroyRenderPass(device, renderPass, nullptr);

	createSwapchain();
	createSwapchainImages();
	//createRenderPass();
	createDepthResources();
	createFrameBuffers();
}

void Swapchain::selectPresentMode()
{
	VkPhysicalDevice physicalDevice = gf3dDevice->GetPhysicalDevice();
	VkSurfaceKHR surface = gf3dDevice->GetSurface();

	u32 presentCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, nullptr);
	gf3d::vector<VkPresentModeKHR> presentModes(presentCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, presentModes.data());

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	//This is the only mode to be guaranteed under the standard
	presentMode = VK_PRESENT_MODE_FIFO_KHR;
	
	//Use fifo as it is more battery efficient for devices with integrated gpus
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
		return;
	}

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

	gf3d::vector< VkCompositeAlphaFlagBitsKHR> alphaFlagBits = {
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

u32 Swapchain::getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities)
{
	//2 is the standard minimum and adding 1 for triple buffering
	u32 numberOfImages = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && numberOfImages > surfaceCapabilities.maxImageCount) {
		numberOfImages = surfaceCapabilities.maxImageCount;
	}

	return numberOfImages;
}

VkSurfaceFormatKHR Swapchain::getSurfaceFormat()
{
	VkPhysicalDevice physicalDevice = gf3dDevice->GetPhysicalDevice();
	VkSurfaceKHR surface = gf3dDevice->GetSurface();

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	gf3d::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

	for (auto& surfaceFormat : surfaceFormats) {

		if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
			return surfaceFormat;
		}
	}

	return surfaceFormats[0];
}

VkFormat Swapchain::findDepthFormat()
{
	VkPhysicalDevice physicalDevice = gf3dDevice->GetPhysicalDevice();
	
	gf3d::vector<VkFormat> depthFormats = { 
		VK_FORMAT_D32_SFLOAT, 
		VK_FORMAT_D32_SFLOAT_S8_UINT, 
		VK_FORMAT_D24_UNORM_S8_UINT 
	};

	for (VkFormat format : depthFormats) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			return format;
		}
	}


	return VK_FORMAT_UNDEFINED;
}

void Swapchain::createSwapchain()
{
	VkDevice device = gf3dDevice->GetDevice();
	VkPhysicalDevice physicalDevice = gf3dDevice->GetPhysicalDevice();
	VkSurfaceKHR surface = gf3dDevice->GetSurface();

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	extent = surfaceCapabilities.currentExtent;

	selectPresentMode();

	VkSurfaceFormatKHR surfaceFormat = getSurfaceFormat();
	colorFormat = surfaceFormat.format;

	depthFormat = findDepthFormat();

	assert(depthFormat != VK_FORMAT_UNDEFINED && "No supported depth format found");

	VkCompositeAlphaFlagBitsKHR compositeAlpha = selectCompositeAlphaFlags(surfaceCapabilities);

	VkSurfaceTransformFlagBitsKHR preTransform = selectSurfaceTransformFlags(surfaceCapabilities);

	u32 numImages = getNumberOfImagesForBuffers(surfaceCapabilities);

	VkSwapchainCreateInfoKHR swapchainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapchainInfo.surface = surface;
	swapchainInfo.oldSwapchain = oldSwapchain;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.compositeAlpha = compositeAlpha;
	swapchainInfo.preTransform = preTransform;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.minImageCount = numImages;
	swapchainInfo.imageFormat = colorFormat;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.clipped = VK_TRUE;

	VK_CHECK(vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain));

	if (oldSwapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
	}

	//keep the old swapchain for when a new one has to be remade
	oldSwapchain = swapchain;

	vkGetSwapchainImagesKHR(device, swapchain, &bufferImageCount, nullptr);
	swapchainImages.resize(bufferImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &bufferImageCount, swapchainImages.data());
}

void Swapchain::createSwapchainImages()
{
	VkDevice device = gf3dDevice->GetDevice();

	swapchainImageViews.resize(bufferImageCount);
	
	for (u32 i = 0; i < bufferImageCount; i++) {

		VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.image = swapchainImages[i];
		viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		viewInfo.format = colorFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;

		VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &swapchainImageViews[i]));
	}
}

void Swapchain::createRenderPass()
{
	VkDevice device = gf3dDevice->GetDevice();

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

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentRef;
	subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependacy = {};
	dependacy.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependacy.dstSubpass = 0;
	dependacy.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.srcAccessMask = 0;
	dependacy.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachments[2] = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	createInfo.attachmentCount = 2;
	createInfo.pAttachments = attachments;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpassDescription;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependacy;

	VK_CHECK(vkCreateRenderPass(device, &createInfo, nullptr, &renderPass));
}

void Swapchain::createDepthResources()
{
	VkDevice device = gf3dDevice->GetDevice();
	VmaAllocator allocator = gf3dDevice->GetAllocator();

	depthAllocatedImage = VulkanImage(
		gf3dDevice,
		VK_IMAGE_TYPE_2D,
		extent.width,
		extent.height,
		depthFormat,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VMA_MEMORY_USAGE_GPU_ONLY);

	depthAllocatedImage.createImageView(gf3dDevice, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Swapchain::createFrameBuffers()
{
	VkDevice device = gf3dDevice->GetDevice();

	frameBuffers.resize(bufferImageCount);
	VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 2;
	framebufferInfo.layers = 1;
	for (size_t i = 0; i < frameBuffers.size(); i++) {
		VkImageView attachments[2] = { swapchainImageViews[i], depthAllocatedImage.view };
		framebufferInfo.pAttachments = attachments;
		VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &frameBuffers[i]));
	}
}