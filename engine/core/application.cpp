#include "gf3d_logger.h"
#include "application.h"
#include <spdlog/stopwatch.h>

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
	f32 aspect = window.getAspectRatio();
	//cam.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 10);
	cam.setPerspectiveProjection(aspect, 0.1f, 100.f);
	cam.setViewDirection(gf3d::vec3(0.f), gf3d::vec3(0.5f, 0.f, 1.f));
	//cam.setViewTarget(gf3d::vec3(-1.f, -2.f, 2.f), gf3d::vec3(0.f, 0.f, 2.5f));

	for (auto& gameObject : gameObjects) {
		gameObject.transform.rotate(1.f, 1.f, 1.f);
		auto finalTransform = gameObject.transform.mat4() * cam.getViewMatrix() * cam.getProjectionMatrix();
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
	newObj.transform.scale = newObj.transform.scale * 0.5f;

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
