#include "core/gf3d_logger.h"
#include "gf3d_window.h"

void Gf3dWindow::init(const i32 Width, const i32 Height, const std::string& Title)
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
	char buffer[256];
	snprintf(buffer, 256, "%s - FPS(%zi)", title.c_str(), fpsCount);
	glfwSetWindowTitle(glfw_window, buffer);
}

void Gf3dWindow::windowResizeCallback(GLFWwindow* window, i32 width, i32 height)
{
	auto appWindow = reinterpret_cast<Gf3dWindow*>(glfwGetWindowUserPointer(window));
	appWindow->width = width;
	appWindow->height = height;
	appWindow->framebufferResized = true;
}
