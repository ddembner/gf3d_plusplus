#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class gf3d_window 
{
public:
	gf3d_window() : width(1080), height(720), title("gf3d vulkan"), glfw_window(nullptr), surface(nullptr) { }
	~gf3d_window() { }
	gf3d_window& operator=(const gf3d_window&) = delete;
	void init();
	void cleanup(VkInstance instance);
	void createWindowSurface(VkInstance instance);
	inline bool windowClosed() { return glfwWindowShouldClose(glfw_window); }
	inline VkSurfaceKHR getWindowSurface() const { return surface; }
private:
	GLFWwindow* glfw_window;
	VkSurfaceKHR surface;
	int width;
	int height;
	const char* title;
};