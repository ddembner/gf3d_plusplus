#include "gf3d_logger.h"
#include "Application.h"
#include <spdlog/stopwatch.h>
#include <memory>

void Application::run()
{
	init();
	while (!isDonePlaying) {
		CalculateFPS();
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
	initScene();
	LOGGER_INFO("Initialized engine successfully");
}

void Application::update()
{
	isDonePlaying = window.windowClosed() || glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE);
	float scaleFactor = sin(glfwGetTime());
	for (auto& gameObject : gameObjects) {
		auto translate = glm::translate(glm::mat4{ 1.f }, glm::vec3{ scaleFactor * 0.5f });
		auto scale = glm::scale(glm::mat4{ 1.f }, glm::vec3{ scaleFactor });
		gameObject.transform = scale * translate;
	}
}

void Application::render()
{
	VkCommandBuffer cmd = renderer.beginFrame();
	if (cmd) {
		renderer.beginRenderPass(cmd);
		renderer.renderObjects(cmd, gameObjects);
		renderer.endRenderPass(cmd);
		renderer.endFrame();
	}
}

void Application::cleanup()
{
	vkDeviceWaitIdle(gf3dDevice.GetDevice());
	destroyGameObjects();
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

void Application::initScene()
{
	GameObject newObj;
	newObj.mesh.vertices = {
		{{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};
	newObj.mesh.allocateMesh(gf3dDevice);
	newObj.color = { 0.1f, 0, 1, 1 };

	gameObjects.push_back(std::move(newObj));
}

void Application::destroyGameObjects()
{
	auto allocator = gf3dDevice.GetAllocator();

	for (auto& gameobject : gameObjects) {
		gameobject.mesh.destroy(allocator);
	}
}
