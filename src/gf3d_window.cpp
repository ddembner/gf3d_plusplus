#include "gf3d_logger.h"
#include "gf3d_window.h"

void Gf3dWindow::init(const int Width, const int Height, const std::string& Title)
{
	width = Width;
	height = Height;
	title = Title;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!glfw_window) {
		LOGGER_ERROR("Failed to initialize GLFW window!");
	}
	assert(glfw_window);
	glfwSetWindowSizeLimits(glfw_window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(glfw_window, this);
	glfwSetFramebufferSizeCallback(glfw_window, windowResizeCallback);
	LOGGER_INFO("Successfully initialized GLFW window");
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

void Gf3dWindow::setFPSWindowTitle(size_t fpsCount)
{
	std::string fpsTitle = title + " - FPS(" + std::to_string(fpsCount) + ')';
	glfwSetWindowTitle(glfw_window, fpsTitle.c_str());
}

void Gf3dWindow::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	auto appWindow = reinterpret_cast<Gf3dWindow*>(glfwGetWindowUserPointer(window));
	appWindow->width = width;
	appWindow->height = height;
	appWindow->framebufferResized = true;
}
