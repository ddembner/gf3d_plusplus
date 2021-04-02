#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class gf3d_window 
{
public:
	gf3d_window();
	~gf3d_window() { }
	gf3d_window& operator=(const gf3d_window&) = delete;
	void init();
	void cleanup(VkInstance instance);
	void createWindowSurface(VkInstance instance);
	inline bool windowClosed() { return glfwWindowShouldClose(glfw_window); }
	inline VkSurfaceKHR getWindowSurface() const { return surface; }
	inline bool wasWindowResized() { return framebufferResized; }
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
private:
	static void windowResizeCallback(GLFWwindow* window, int width, int height);
private:
	GLFWwindow* glfw_window;
	VkSurfaceKHR surface;
	int width;
	int height;
	const char* title;
	bool framebufferResized = false;
};