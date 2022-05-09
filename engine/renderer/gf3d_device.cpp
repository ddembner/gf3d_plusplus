#include "core/gf3d_logger.h"
#include "gf3d_device.h"
#include "gf3d_validations.h"
#include "vulkan_functions.h"
#include "containers/vector.hpp"

void Gf3dDevice::init(Gf3dWindow* window)
{
	assert(window);
	gf3dWindow = window;
	createInstance();
	if (isValidationLayersEnabled()) {
		setupDebugCallback();
		LOGGER_DEBUG("Validation layers are enabled");
	}
	surface = window->createWindowSurface(instance);
	findPhysicalDevice();
	createLogicalDevice();
	createMemoryAllocator();
	createCommandPool();
}

void Gf3dDevice::createSurface()
{
	surface = gf3dWindow->createWindowSurface(instance);
}

void Gf3dDevice::cleanup()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	vmaDestroyAllocator(allocator);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void Gf3dDevice::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_API_VERSION_1_2;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;
	appInfo.pApplicationName = "";
	appInfo.pEngineName = "";

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.pApplicationInfo = &appInfo;

	gf3d::vector<const char*> validationLayers = getValidationLayers();
	if (isValidationLayersEnabled()) {

		instanceInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		instanceInfo.enabledLayerCount = 0;
	}

	u32 glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	gf3d::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (isValidationLayersEnabled()) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	instanceInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &instance));

	if (!instance) {
		LOGGER_ERROR("Failed to create vulkan insatnce");
	}
}

void Gf3dDevice::setupDebugCallback()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = gf3dVkValidationDebugCallback;

	VK_CHECK(CreateDebugUtilsMessengerEXT(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &callback));

	if (!callback) {
		LOGGER_ERROR("Failed to create vulkan debug callbacks");
	}
}

void Gf3dDevice::findPhysicalDevice()
{
	u32 deviceCount;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	assert(deviceCount > 0);

	gf3d::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (u32 i = 0; i < devices.size(); i++) {

		vkGetPhysicalDeviceProperties(devices[i], &physicalDeviceInfo.deviceProperties);
		if (physicalDeviceInfo.deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			physicalDevice = devices[i];
			vkGetPhysicalDeviceFeatures(devices[i], &physicalDeviceInfo.deviceFeatures);
			vkGetPhysicalDeviceMemoryProperties(devices[i], &physicalDeviceInfo.deviceMemoryProperties);
			LOGGER_DEBUG("Picking discrete gpu: {}", physicalDeviceInfo.deviceProperties.deviceName);
			return;
		}

	}

	//default to first device
	physicalDevice = devices[0];
	vkGetPhysicalDeviceProperties(devices[0], &physicalDeviceInfo.deviceProperties);
	vkGetPhysicalDeviceFeatures(devices[0], &physicalDeviceInfo.deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(devices[0], &physicalDeviceInfo.deviceMemoryProperties);
	LOGGER_DEBUG("Using fallback gpu: {}", physicalDeviceInfo.deviceProperties.deviceName);
}

void Gf3dDevice::createLogicalDevice()
{
	const float queuePriority = 1.0f;
	gf3d::vector<VkDeviceQueueCreateInfo> queueInfos;
	gf3d::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VkPhysicalDeviceFeatures enabledDeviceFeatures = {};

	queueIndices.graphics = findQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
	VkDeviceQueueCreateInfo graphicsInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	graphicsInfo.queueCount = 1;
	graphicsInfo.pQueuePriorities = &queuePriority;
	graphicsInfo.queueFamilyIndex = queueIndices.graphics;
	queueInfos.push_back(graphicsInfo);

	queueIndices.compute = findQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT, false);
	if (queueIndices.graphics != queueIndices.compute) {
		VkDeviceQueueCreateInfo computeInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		computeInfo.queueCount = 1;
		computeInfo.pQueuePriorities = &queuePriority;
		computeInfo.queueFamilyIndex = queueIndices.compute;
		queueInfos.push_back(computeInfo);
	}

	queueIndices.transfer = findQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, false);
	if (queueIndices.transfer != queueIndices.graphics && queueIndices.transfer != queueIndices.compute) {
		VkDeviceQueueCreateInfo transferInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		transferInfo.queueCount = 1;
		transferInfo.pQueuePriorities = &queuePriority;
		transferInfo.queueFamilyIndex = queueIndices.transfer;
		queueInfos.push_back(transferInfo);
	}

	LOGGER_DEBUG("Graphics queue index: {}", queueIndices.graphics);
	LOGGER_DEBUG("Compute queue index: {}", queueIndices.compute);
	LOGGER_DEBUG("Transfer queue index: {}", queueIndices.transfer);

	VkDeviceCreateInfo deviceInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	deviceInfo.queueCreateInfoCount = static_cast<u32>(queueInfos.size());
	deviceInfo.pQueueCreateInfos = queueInfos.data();
	deviceInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceInfo.pEnabledFeatures = &enabledDeviceFeatures;

	VK_CHECK(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device));

	if (!physicalDevice) {
		LOGGER_ERROR("Failed to find suitable gpu");
	}

	vkGetDeviceQueue(device, queueIndices.graphics, 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueIndices.compute, 0, &computeQueue);
	vkGetDeviceQueue(device, queueIndices.transfer, 0, &transferQueue);
}

void Gf3dDevice::createMemoryAllocator()
{
	VmaAllocatorCreateInfo createInfo = {};
	createInfo.instance = instance;
	createInfo.physicalDevice = physicalDevice;
	createInfo.device = device;
	createInfo.vulkanApiVersion = VK_API_VERSION_1_2;

	vmaCreateAllocator(&createInfo, &allocator);

	if (!allocator) {
		LOGGER_ERROR("Failed to create allocator");
	}
}

void Gf3dDevice::createCommandPool()
{
	VkCommandPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	createInfo.queueFamilyIndex = queueIndices.graphics;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VK_CHECK(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool));

	if (!allocator) {
		LOGGER_ERROR("Failed to create command pool");
	}
}

u32 Gf3dDevice::findQueueFamilyIndex(VkQueueFlags queueFlag, bool singleQueue)
{
	u32 propertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, nullptr);
	gf3d::vector<VkQueueFamilyProperties> queueFamilyProperties(propertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, queueFamilyProperties.data());

	//Get dedicated compute queue
	if (queueFlag & VK_QUEUE_COMPUTE_BIT) {

		u32 queueIndex = 0;
		for (u32 i = 0; i < static_cast<u32>(queueFamilyProperties.size()); i++) {
			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (presentSupport) {
				//Have compute and graphics queue be the same
				if (singleQueue && queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT &&
					queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					return i;
				}
				//Have compute be its own queue
				if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT &&
					!(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
						return i;
				}
				queueIndex = i;
			}
		}

		return queueIndex;
	}

	//Get dedicated transfer queue
	if (queueFlag & VK_QUEUE_TRANSFER_BIT) {

		for (u32 i = 0; i < static_cast<u32>(queueFamilyProperties.size()); i++) {

			if (singleQueue && queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT &&
				queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				return i;
			}

			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				return i;
			}
		}
	}

	//Get the index from any queue that supports flag
	for (u32 i = 0; i < queueFamilyProperties.size(); i++) {

		if (queueFamilyProperties[i].queueFlags & queueFlag) {
			return i;
		}
	}

	LOGGER_ERROR("Could not find a matching family index for the queue");

	return UINT32_MAX;
}
