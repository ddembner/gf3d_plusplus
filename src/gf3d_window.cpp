#include "gf3d_window.h"

void gf3d_window::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
