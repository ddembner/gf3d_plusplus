#include "gf3d_window.h"
#include <cassert>

Gf3dWindow::Gf3dWindow(const int Width, const int Height, const std::string& Title) : width(Width), height(Height), title(Title)
{
	init();
}

void Gf3dWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	assert(glfw_window && "Failed to create glfw window");
	glfwSetWindowSizeLimits(glfw_window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(glfw_window, this);
	glfwSetFramebufferSizeCallback(glfw_window, windowResizeCallback);
}

void Gf3dWindow::cleanup()
{
	glfwDestroyWindow(glfw_window);
	glfwTerminate();
}

VkSurfaceKHR Gf3dWindow::createWindowSurface(VkInstance instance)
{
	VkSurfaceKHR newSurface;
	glfwCreateWindowSurface(instance, glfw_window, nullptr, &newSurface);
	return newSurface;
}

void Gf3dWindow::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	auto appWindow = reinterpret_cast<Gf3dWindow*>(glfwGetWindowUserPointer(window));
	appWindow->width = width;
	appWindow->height = height;
	appWindow->framebufferResized = true;
}
