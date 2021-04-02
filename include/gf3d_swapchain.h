#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

class Swapchain
{
public:
	Swapchain();
	~Swapchain();
	void init(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
	void cleanup(VkDevice device);
	uint32_t imageCount() const;
	VkFormat getColorFormat() const;
	VkExtent2D getExtent() const;
	VkSwapchainKHR getSwapchain() const;
	VkRenderPass getRenderPass() const;
	VkFramebuffer getFrameBuffer(uint32_t index);
	void recreate(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
private:
	void selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	VkCompositeAlphaFlagBitsKHR selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	uint32_t getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceFormatKHR getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void createRenderPass(VkDevice device);
	void createFrameBuffers(VkDevice device);
private:
	VkExtent2D extent;
	VkPresentModeKHR presentMode;
	VkSwapchainKHR swapchain;
	VkSwapchainKHR oldSwapchain;
	uint32_t bufferImageCount;
	VkFormat colorFormat;
	VkFormat depthFormat;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> frameBuffers;

	struct SwapchainBuffer {
		VkImage image;
		VkImageView view;
	};

	std::vector<SwapchainBuffer> buffers;

};