#include "Application.h"
#include <iostream>
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
	window = Gf3dWindow::Create();
	renderer.init(window.get());
	renderer.loadModel();
}

void Application::update()
{
	isDonePlaying = window->windowClosed() || glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE);
}

void Application::render()
{
	renderer.draw();
}

void Application::cleanup()
{
	renderer.cleanup();
	window->cleanup();
}