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
	glfwInit();
	renderer.createMaterial("D:/Programming/gf3d++/gf3d++/shaders/vert.spv", "D:/Programming/gf3d++/gf3d++/shaders/frag.spv");
	renderer.createMaterial("D:/Programming/gf3d++/gf3d++/shaders/vert.spv", "D:/Programming/gf3d++/gf3d++/shaders/frag.spv");
}

void Application::update()
{
	isDonePlaying = renderer.DidWindowClosed();
}

void Application::render()
{
	renderer.draw();
}

void Application::cleanup()
{
	renderer.cleanup();
}