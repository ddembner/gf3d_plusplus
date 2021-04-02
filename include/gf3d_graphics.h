#pragma once

#include <cassert>
#include <unordered_map>
#include "gf3d_swapchain.h"
#include "gf3d_window.h"
#include "gf3d_pipeline.h"

class Gf3dGraphics
{
public:
	Gf3dGraphics() {}
	~Gf3dGraphics() {}
	void init();
	void cleanup();
	inline bool DidWindowClosed() { return window.windowClosed(); }
	void draw();
private:
	//Vulkan Specifics

	VkInstance instance;
	VkDebugUtilsMessengerEXT callback;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	Swapchain swapchain;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderCompleteSemaphores;
	std::vector<VkFence> imagesInFlight;
	std::vector<VkFence> inFlightFences;
	std::unordered_map<std::string, Material> materials;
	struct {
		uint32_t graphics;
		uint32_t compute;
		uint32_t transfer;
	} queueIndices;

	bool isDiscreteGpu = false;
	const uint32_t MAX_FRAMES_INFLIGHT = 2;
	size_t currentFrame = 0;

	
	
	//GLFW Window Specifics

	gf3d_window window;


public:
	void createMaterial(const std::string& vertPath, const std::string& fragPath);
private:
	void initVulkan();
	void initWindow();
	void cleanWindow();
	void cleanMaterials();

	//Vulkan Specifics

	void createInstance();
	void setupDebugCallback();
	void findPhysicalDevice();
	void createLogicalDevice();
	uint32_t findQueueFamilyIndex(VkQueueFlags queueFlag);
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void recordCommandBuffer(uint32_t imageIndex);
	void recreateSwapChain();
};
