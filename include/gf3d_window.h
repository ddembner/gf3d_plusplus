#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include "NonCopyable.h"

class Gf3dWindow : NonCopyable
{
public:
	Gf3dWindow(const int Width = 1280, const int Height = 720, const std::string& Title = "Gf3d++ Engine");
	~Gf3dWindow() { }

	void cleanup();
	VkSurfaceKHR createWindowSurface(VkInstance instance);
	GLFWwindow* getWindow() const { return glfw_window; }
	inline bool windowClosed() { return glfwWindowShouldClose(glfw_window); }
	inline bool wasWindowResized() { return framebufferResized; }
	VkExtent2D getExtent() const { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
private:
	void init();
	static void windowResizeCallback(GLFWwindow* window, int width, int height);
private:
	GLFWwindow* glfw_window = nullptr;
	int width;
	int height;
	std::string title;
	bool framebufferResized = false;
};