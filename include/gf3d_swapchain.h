#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "gf3d_device.h"

class Swapchain
{
public:
	Swapchain() {}
	~Swapchain() {}
	void init(Gf3dDevice& gf3dDevice);
	void cleanup(VkDevice device);
	uint32_t imageCount() const { return bufferImageCount; }
	VkFormat getColorFormat() const { return colorFormat; }
	VkExtent2D getExtent() const { return extent; }
	VkSwapchainKHR getSwapchain() const { return swapchain; }
	VkRenderPass getRenderPass() const { return renderPass; }
	VkFramebuffer getFrameBuffer(uint32_t index) { return frameBuffers[index]; }
	void recreate(Gf3dDevice& gf3dDevice);
private:
	void selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	VkCompositeAlphaFlagBitsKHR selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	uint32_t getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceFormatKHR getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void createRenderPass(VkDevice device);
	void createFrameBuffers(VkDevice device);
private:
	VkExtent2D extent = { 0 };
	VkPresentModeKHR presentMode;
	VkSwapchainKHR swapchain;
	VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE;
	uint32_t bufferImageCount = 0;
	VkFormat colorFormat = VK_FORMAT_UNDEFINED;
	VkFormat depthFormat = VK_FORMAT_UNDEFINED;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> frameBuffers;

	struct SwapchainBuffer {
		VkImage image;
		VkImageView view;
	};

	std::vector<SwapchainBuffer> buffers;

};