#include "gf3d_logger.h"
#include "Application.h"
void Application::run()
{
	init();
	while (!isDonePlaying) {
		glfwPollEvents();
		update();
		render();
	}

	cleanup();
}

void Application::init()
{
	Logger::init();
	window.init();
	gf3dDevice.init(&window);
	renderer.init(&window, &gf3dDevice);
	renderer.loadModel();
	LOGGER_INFO("Initialized engine successfully");
}

void Application::update()
{
	isDonePlaying = window.windowClosed() || glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE);
}

void Application::render()
{
	renderer.draw();
}

void Application::cleanup()
{
	renderer.cleanup();
	gf3dDevice.cleanup();
	window.cleanup();
}