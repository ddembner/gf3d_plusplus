#pragma once
#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wnullability-completeness"
#endif // __APPLE__

#include <vk_mem_alloc.h>
#include "platform/gf3d_window.h"

class Gf3dDevice : NonCopyable
{
public:
	Gf3dDevice() = default;
	~Gf3dDevice() = default;
	void init(Gf3dWindow* window);
	void createSurface();
	void cleanup();
public:
	const VkInstance& GetInstance() { return instance; }
	VkDevice GetDevice() const { return device; }
	const VkPhysicalDevice& GetPhysicalDevice() { return physicalDevice; }
	const VmaAllocator GetAllocator() { return allocator; }
	const VkSurfaceKHR GetSurface() { return surface; }
	const VkQueue GetGraphicsQueue() { return graphicsQueue; }
	const VkQueue GetComputeQueue() { return computeQueue; }
	const VkQueue GetTransferQueue() { return transferQueue; }
	const VkCommandPool GetCommandPool() { return commandPool; }
	const u32 GetQueueIndex (VkQueueFlags queueFlag);
private:
	void createInstance();
	void setupDebugCallback();
	void findPhysicalDevice();
	void createLogicalDevice();
	void createMemoryAllocator();
	void createCommandPool();
	u32 findQueueFamilyIndex(VkQueueFlags queueFlag, bool singleQueue = true);
private:
	VkInstance instance = 0;
	VkDebugUtilsMessengerEXT callback = 0;
	VkPhysicalDevice physicalDevice = 0;
	VkDevice device = 0;
	VmaAllocator allocator = 0;
	VkSurfaceKHR surface = 0;
	VkQueue graphicsQueue = 0;
	VkQueue computeQueue = 0;
	VkQueue transferQueue = 0;
	VkCommandPool commandPool = 0;

	struct PhysicalDeviceInfo {
		VkPhysicalDeviceFeatures deviceFeatures;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	} physicalDeviceInfo;

	struct QueueIndices{
		u32 graphics;
		u32 compute;
		u32 transfer;
	} queueIndices;

	Gf3dWindow* gf3dWindow;
};