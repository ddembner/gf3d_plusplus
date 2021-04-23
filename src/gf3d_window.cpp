#include "gf3d_window.h"
#include <iostream>

void Gf3dWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!glfw_window) {
		std::cout << "Failed to create window" << std::endl;
		exit(-1);
	}
	glfwSetWindowSizeLimits(glfw_window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(glfw_window, this);
	glfwSetFramebufferSizeCallback(glfw_window, windowResizeCallback);
}

std::unique_ptr<Gf3dWindow> Gf3dWindow::Create(const int width, const int height, const std::string& title)
{
	auto newWindow = std::make_unique<Gf3dWindow>(width, height, title);
	newWindow->init();
	return newWindow;
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
