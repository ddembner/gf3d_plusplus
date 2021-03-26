#include "gf3d_graphics.h"
#include "gf3d_validations.h"
#include "vulkan_functions.h"
#include <iostream>

using std::cout;
using std::endl;

Gf3dGraphics::Gf3dGraphics() 
{
	initWindow();
	initVulkan();
}

Gf3dGraphics::~Gf3dGraphics() 
{
}

void Gf3dGraphics::cleanup()
{
	vkDeviceWaitIdle(device);
	cleanMaterials();
	for (uint32_t i = 0; i < MAX_FRAMES_INFLIGHT; i++) {
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderCompleteSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	vkDestroyCommandPool(device, commandPool, nullptr);
	swapchain.cleanup(device);
	cleanWindow();
	vkDestroyDevice(device, nullptr);
	DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	vkDestroyInstance(instance, nullptr);

	cout << "Successfully cleaned graphics" << endl;
}

void Gf3dGraphics::draw()
{
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapchain.getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderCompleteSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_INFLIGHT;
}

//Creates and initializes the glfw window
void Gf3dGraphics::initWindow() 
{
	glfwInit();
	window.init();
}

void Gf3dGraphics::cleanWindow()
{
	window.cleanup(instance);
	cout << "Successfully cleaned window" << endl;
}

void Gf3dGraphics::cleanMaterials()
{
	for (auto& material : materials) {
		material.second.freeMaterial(device);
	}
}

void Gf3dGraphics::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;
	appInfo.pApplicationName = "";
	appInfo.pEngineName = "";

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> validationLayers = getValidationLayers();
	if (isValidationLayersEnabled()) {

		instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		instanceInfo.enabledLayerCount = 0;
	}

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (isValidationLayersEnabled()) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &instance));

	cout << "successfully created vulkan instance" << endl;
}

void Gf3dGraphics::setupDebugCallback()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;

	VK_CHECK(CreateDebugUtilsMessengerEXT(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &callback));
}

void Gf3dGraphics::findPhysicalDevice()
{
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	assert(deviceCount > 0);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (uint32_t i = 0; i < devices.size(); i++) {

		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
		if (deviceProperties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			physicalDevice = devices[i];
			isDiscreteGpu = true;
			vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);
			vkGetPhysicalDeviceMemoryProperties(devices[i], &deviceMemoryProperties);
			std::cout << "Picking discrete gpu: " << deviceProperties.deviceName << std::endl;
			return;
		}

	}

	//default to first device
	physicalDevice = devices[0];
	vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);
	vkGetPhysicalDeviceFeatures(devices[0], &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(devices[0], &deviceMemoryProperties);
	std::cout << "Using fallback gpu: " << deviceProperties.deviceName << std::endl;

}

void Gf3dGraphics::createLogicalDevice()
{
	const float queuePriority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VkPhysicalDeviceFeatures enabledDeviceFeatures = {};

	queueIndices.graphics = findQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
	VkDeviceQueueCreateInfo graphicsInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	graphicsInfo.queueCount = 1;
	graphicsInfo.pQueuePriorities = &queuePriority;
	graphicsInfo.queueFamilyIndex = queueIndices.graphics;
	queueInfos.push_back(graphicsInfo);

	queueIndices.compute = findQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
	if (queueIndices.graphics != queueIndices.compute) {
		VkDeviceQueueCreateInfo computeInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		graphicsInfo.queueCount = 1;
		graphicsInfo.pQueuePriorities = &queuePriority;
		graphicsInfo.queueFamilyIndex = queueIndices.compute;
		queueInfos.push_back(computeInfo);
	}

	VkDeviceCreateInfo deviceInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
	deviceInfo.pQueueCreateInfos = queueInfos.data();
	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceInfo.pEnabledFeatures = &enabledDeviceFeatures;

	VK_CHECK(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device));

	vkGetDeviceQueue(device, queueIndices.graphics, 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueIndices.compute, 0, &presentQueue);
}

uint32_t Gf3dGraphics::findQueueFamilyIndex(VkQueueFlags flag) {


	uint32_t propertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(propertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, queueFamilyProperties.data());

	if (flag & VK_QUEUE_COMPUTE_BIT) {

		uint32_t queueIndex = 0;
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) { 
				VkBool32 presentSupport;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, window.getWindowSurface(), &presentSupport);
				if (presentSupport) {
					return i;
				}

				queueIndex = i;
			}
		}

		return queueIndex;
	}

	//Get dedicated transfer queue
	if (flag & VK_QUEUE_TRANSFER_BIT) {

		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {

			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				return i;
			}
		}
	}

	//Get the index from any queue that supports flag
	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {

		if (queueFamilyProperties[i].queueFlags & flag) {
			return i;
		}
	}

	std::cout << "Could not find a matching family index for the queue" << std::endl;
	return UINT32_MAX;
}

void Gf3dGraphics::createCommandPool()
{
	VkCommandPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	createInfo.queueFamilyIndex = queueIndices.graphics;
	VK_CHECK(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool));
}

void Gf3dGraphics::createCommandBuffers()
{
	commandBuffers.resize(swapchain.imageCount());
	
	VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
}

void Gf3dGraphics::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_INFLIGHT);
	renderCompleteSemaphores.resize(MAX_FRAMES_INFLIGHT);
	inFlightFences.resize(MAX_FRAMES_INFLIGHT);
	imagesInFlight.resize(swapchain.imageCount());

	VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

	VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < MAX_FRAMES_INFLIGHT; i++) {
		VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
		VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderCompleteSemaphores[i]));
		VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]));
	}
}

void Gf3dGraphics::createMaterial(const std::string& vertPath, const std::string& fragPath)
{
	//TODO: Make better system in which materials are named/made
	std::string uniqueMaterialName = vertPath + fragPath;
	if (materials.find(uniqueMaterialName) != materials.end()) {
		cout << "Material already exists" << std::endl;
		return;
	}
	Material material(swapchain, device, vertPath, fragPath);
	materials[uniqueMaterialName] = material;
}

//Initializes the vulkan api
void Gf3dGraphics::initVulkan() 
{
	if (isValidationLayersEnabled() && !checkValidationLayerSupport()) {
		cout << "validation layers requested, but not available" << endl;
		return;
	}
	
	createInstance();

	if (isValidationLayersEnabled()) {
		setupDebugCallback();
		cout << "validation layers are on" << endl;
	}

	window.createWindowSurface(instance);
	
	findPhysicalDevice();

	createLogicalDevice();

	swapchain.init(instance, physicalDevice, device, window.getWindowSurface());
	
	createCommandPool();

	createCommandBuffers();

	createSyncObjects();
}