#include "gf3d_graphics.h"
#include <iostream>

const char* app_name = "gf3d vulkan";

Gf3dGraphics::Gf3dGraphics(int width, int height) {

	window.width = width;
	window.height = height;
	initVulkan();
	initWindow();
}

Gf3dGraphics::~Gf3dGraphics() {

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window.glfw_window);

	glfwTerminate();

	std::cout << "Successfully cleaned everything" << std::endl;
}

//Creates and initializes the glfw window
void Gf3dGraphics::initWindow() {

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window.glfw_window = glfwCreateWindow(window.width, window.height, app_name, nullptr, nullptr);
}

//Returns the glfw this application uses
GLFWwindow* Gf3dGraphics::getWindow() {
	return window.glfw_window;
}

//Initializes the vulkan api
void Gf3dGraphics::initVulkan() {
	
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;
	appInfo.pApplicationName = app_name;
	appInfo.pEngineName = app_name;

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** extensions;
	extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	instanceInfo.enabledExtensionCount = glfwExtensionCount;
	instanceInfo.ppEnabledExtensionNames = extensions;
	instanceInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vulkan instance");
	}
	else {
		std::cout << "successfully created vulkan instance" << std::endl;
	}
}