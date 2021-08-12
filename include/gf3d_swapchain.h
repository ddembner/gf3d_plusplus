#pragma once
#include <vector>
#include "gf3d_device.h"
#include "vulkan_types.h"

class Swapchain
{
public:
	Swapchain() {}
	~Swapchain() {}
	void init(Gf3dDevice* device);
	void cleanup();
	uint32_t imageCount() const { return bufferImageCount; }
	VkFormat getColorFormat() const { return colorFormat; }
	VkExtent2D getExtent() const { return extent; }
	VkSwapchainKHR getSwapchain() const { return swapchain; }
	VkRenderPass getRenderPass() const { return renderPass; }
	VkFramebuffer getFrameBuffer(uint32_t index) { return frameBuffers[index]; }
	void recreate();
private:
	void selectPresentMode();
	VkCompositeAlphaFlagBitsKHR selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	uint32_t getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities);
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
	uint32_t bufferImageCount = 0;
	VkFormat colorFormat = VK_FORMAT_UNDEFINED;
	VkFormat depthFormat = VK_FORMAT_UNDEFINED;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> frameBuffers;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	AllocatedImage depthAllocatedImage;
	VkImageView depthImageView;
};