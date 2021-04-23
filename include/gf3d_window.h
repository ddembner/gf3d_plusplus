#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

class Gf3dWindow 
{
public:
	Gf3dWindow(const int Width, const int Height, const std::string& Title) : width(Width), height(Height), title(Title) {}
	~Gf3dWindow() { }
	Gf3dWindow(const Gf3dWindow&) = delete;
	Gf3dWindow& operator=(const Gf3dWindow&) = delete;
	static std::unique_ptr<Gf3dWindow> Create(const int width = 1280, const int height = 720, const std::string& title = "gf3d++");
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