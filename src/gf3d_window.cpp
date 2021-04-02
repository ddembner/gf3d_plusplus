#include "gf3d_window.h"

gf3d_window::gf3d_window() : width(800), height(600), title("gf3d vulkan"), glfw_window(nullptr), surface(nullptr)
{
}

void gf3d_window::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowSizeLimits(glfw_window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(glfw_window, this);
	glfwSetFramebufferSizeCallback(glfw_window, windowResizeCallback);
}

void gf3d_window::cleanup(VkInstance instance)
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
	glfwDestroyWindow(glfw_window);
	glfwTerminate();
}

void gf3d_window::createWindowSurface(VkInstance instance)
{
	glfwCreateWindowSurface(instance, glfw_window, nullptr, &surface);
}

void gf3d_window::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	auto appWindow = reinterpret_cast<gf3d_window*>(glfwGetWindowUserPointer(window));
	appWindow->width = width;
	appWindow->height = height;
	appWindow->framebufferResized = true;
}
