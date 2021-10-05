#pragma once
#include "gf3d_device.h"
#include "vulkan_types.h"
#include "containers/vector.hpp"

class Swapchain
{
public:
	Swapchain() {}
	~Swapchain() {}
	void init(Gf3dDevice* device);
	void cleanup();
	u32 imageCount() const { return bufferImageCount; }
	VkFormat getColorFormat() const { return colorFormat; }
	VkExtent2D getExtent() const { return extent; }
	VkSwapchainKHR getSwapchain() const { return swapchain; }
	VkRenderPass getRenderPass() const { return renderPass; }
	VkFramebuffer getFrameBuffer(u32 index) { return frameBuffers[index]; }
	void recreate();
private:
	void selectPresentMode();
	VkCompositeAlphaFlagBitsKHR selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	u32 getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceFormatKHR getSurfaceFormat();
	VkFormat findDepthFormat();
	void createSwapchain();
	void createSwapchainImages();
	void createRenderPass();
	void createDepthResources();
	void createFrameBuffers();
private:
	Gf3dDevice* gf3dDevice;
	VkExtent2D extent = { 0 };
	VkPresentModeKHR presentMode;
	VkSwapchainKHR swapchain;
	VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE;
	u32 bufferImageCount = 0;
	VkFormat colorFormat = VK_FORMAT_UNDEFINED;
	VkFormat depthFormat = VK_FORMAT_UNDEFINED;
	VkRenderPass renderPass;
	gf3d::vector<VkFramebuffer> frameBuffers;

	gf3d::vector<VkImage> swapchainImages;
	gf3d::vector<VkImageView> swapchainImageViews;
	AllocatedImage depthAllocatedImage;
	VkImageView depthImageView;
};