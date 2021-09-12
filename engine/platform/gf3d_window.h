#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>
#include "core/NonCopyable.h"
#include "defines.hpp"

class Gf3dWindow : NonCopyable
{
public:
	Gf3dWindow() = default;
	void init(const int Width = 1280, const int Height = 720, const std::string& Title = "Gf3d++ Engine");
	void cleanup();
	VkSurfaceKHR createWindowSurface(VkInstance instance);
	GLFWwindow* getWindow() const { return glfw_window; }
	inline bool windowClosed() { return glfwWindowShouldClose(glfw_window); }
	inline bool wasWindowResized() { return framebufferResized; }
	VkExtent2D getExtent() const { return { static_cast<u32>(width), static_cast<u32>(height) }; }
	float getAspectRatio() const { return static_cast<float>(width) / static_cast<float>(height); }
	void setFPSWindowTitle(size_t fpsCount);
	const std::string& getTitle() const { return title; }
private:
	static void windowResizeCallback(GLFWwindow* window, int width, int height);
private:
	GLFWwindow* glfw_window = nullptr;
	int width = 0;
	int height = 0;
	std::string title;
	bool framebufferResized = false;
};