#include "gf3d_logger.h"
#include "Application.h"
#include <spdlog/stopwatch.h>

void Application::run()
{
	init();
	while (!isDonePlaying) {
		glfwPollEvents();
		update();
		render();
		CalculateFPS();
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
	if (VkCommandBuffer cmd = renderer.beginFrame()) {
		renderer.draw();
	}
}

void Application::cleanup()
{
	renderer.cleanup();
	gf3dDevice.cleanup();
	window.cleanup();
	Logger::shutdown();
}

void Application::CalculateFPS()
{
	static spdlog::stopwatch sw;
	static size_t fpsCount = 0;
	fpsCount++;
	if (sw.elapsed().count() > 1.0) {
		window.setFPSWindowTitle(fpsCount);
		fpsCount = 0;
		sw.reset();
	}
}
