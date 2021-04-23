#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "gf3d_window.h"

class Gf3dDevice
{
public:
	Gf3dDevice() = default;
	Gf3dDevice(const Gf3dDevice&) = delete;
	Gf3dDevice& operator=(const Gf3dDevice&) = delete;
	void init(Gf3dWindow* window);
	void createSurface(Gf3dWindow* window);
	void cleanup();
public:
	const VkInstance& GetInstance() { return instance; }
	const VkDevice& GetDevice() { return device; }
	const VkPhysicalDevice& GetPhysicalDevice() { return physicalDevice; }
	const VmaAllocator GetAllocator() { return allocator; }
	const VkSurfaceKHR GetSurface() { return surface; }
	const VkQueue GetGraphicsQueue() { return graphicsQueue; }
	const VkQueue GetPresentQueue() { return presentQueue; }
	const VkQueue GetTransferQueue() { return transferQueue; }
	const uint32_t GetQueueIndex (VkQueueFlags queueFlag);
private:
	void createInstance();
	void setupDebugCallback();
	void findPhysicalDevice();
	void createLogicalDevice();
	void createMemoryAllocator();
	uint32_t findQueueFamilyIndex(VkQueueFlags queueFlag);
private:
	VkInstance instance;
	VkDebugUtilsMessengerEXT callback;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VmaAllocator allocator;
	VkSurfaceKHR surface;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue transferQueue;

	struct PhysicalDeviceInfo {
		VkPhysicalDeviceFeatures deviceFeatures;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	} physicalDeviceInfo;

	struct QueueIndices{
		uint32_t graphics;
		uint32_t compute;
		uint32_t transfer;
	} queueIndices;
};