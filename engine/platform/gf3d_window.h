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
	void init(const i32 Width = 1280, const i32 Height = 720, const std::string& Title = "Gf3d++ Engine");
	void cleanup();
	VkSurfaceKHR createWindowSurface(VkInstance instance);
	GLFWwindow* getWindow() const { return glfw_window; }
	inline bool windowClosed() { return glfwWindowShouldClose(glfw_window); }
	inline bool wasWindowResized() { return framebufferResized; }
	VkExtent2D getExtent() const { return { static_cast<u32>(width), static_cast<u32>(height) }; }
	float getAspectRatio() const { return static_cast<f32>(width) / static_cast<f32>(height); }
	void setFPSWindowTitle(size_t fpsCount);
	const std::string& getTitle() const { return title; }
private:
	static void windowResizeCallback(GLFWwindow* window, i32 width, i32 height);
private:
	GLFWwindow* glfw_window = nullptr;
	i32 width = 0;
	i32 height = 0;
	std::string title;
	bool framebufferResized = false;
};