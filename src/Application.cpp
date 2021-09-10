#include "gf3d_logger.h"
#include "Application.h"
#include <spdlog/stopwatch.h>
#include <bitset>
#include <algorithm>

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
	materialSystem = std::make_unique<MaterialSystem>();
	materialSystem->init(&gf3dDevice, renderer.getSwapchainRenderPass());
	initScene();
	LOGGER_INFO("Initialized engine successfully");
}

void Application::update()
{
	isDonePlaying = window.windowClosed() || glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE);
	float aspect = window.getAspectRatio();
	//cam.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
	cam.setPerspectiveProjection(aspect, 0.1f, 100.f);
	cam.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
	//cam.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));
	for (auto& gameObject : gameObjects) {
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y + 0.01f, glm::two_pi<float>());
		gameObject.transform.rotation.x = glm::mod(gameObject.transform.rotation.x + 0.005f, glm::two_pi<float>());
		auto finalTransform = cam.getViewProjectionMatrix() * gameObject.transform.mat4();
		gameObject.material->pushUpdate("mvp", &finalTransform);
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
	materialSystem->destroy();
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
		// left face (white)
	  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
	  {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
	  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

	  // right face (yellow)
	  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
	  {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
	  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

	  // top face (orange, remember y axis points down)
	  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	  {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

	  // bottom face (red)
	  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
	  {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
	  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

	  // nose face (blue)
	  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	  {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

	  // tail face (green)
	  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	  {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	};
	newObj.mesh.allocateMesh(gf3dDevice);
	newObj.color = { 0.1f, 0, 1, 1 };

	newObj.material = materialSystem->create(ASSETS_PATH "shaders/test.shader");

	//newObj.material->pushUpdate("color", &newObj.color);

	newObj.transform.position.z = 2.5f;
	newObj.transform.scale *= 0.5f;

	gameObjects.push_back(std::move(newObj));

	//cam.position = glm::vec3( 0, 0, -1 );
	//cam.OnUpdate();
}

void Application::destroyGameObjects()
{
	auto allocator = gf3dDevice.GetAllocator();

	for (auto& gameobject : gameObjects) {
		gameobject.mesh.destroy(allocator);
	}
}
