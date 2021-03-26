#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

class Swapchain
{
public:
	Swapchain();
	~Swapchain();
	void init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
	void cleanup(VkDevice device);
	uint32_t imageCount() const;
	VkFormat getColorFormat() const;
	VkExtent2D getExtent() const;
	VkSwapchainKHR getSwapchain() const;
private:
	void selectPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	VkCompositeAlphaFlagBitsKHR selectCompositeAlphaFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR selectSurfaceTransformFlags(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	uint32_t getNumberOfImagesForBuffers(VkSurfaceCapabilitiesKHR surfaceCapabilities);
	VkSurfaceFormatKHR getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
private:
	VkExtent2D extent;
	VkPresentModeKHR presentMode;
	VkSwapchainKHR swapchain;
	VkSwapchainKHR oldSwapchain;
	std::vector<VkImage> images;
	uint32_t bufferImageCount;
	VkFormat colorFormat;
	VkFormat depthFormat;

	struct SwapchainBuffer {
		VkImage image;
		VkImageView view;
	};

	std::vector<SwapchainBuffer> buffers;

};