#include "gf3d_logger.h"
#include "application.h"
#include <spdlog/stopwatch.h>
#include "gf3d_memory.h"

void Application::run()
{	
	init();
	while (!isDonePlaying) {
		appTime.update();
		glfwPollEvents();
		update();
		render();
		CalculateFPS();
	}
	cleanup();
}

void Application::init()
{
	gf3d::initDynamicMemorySystem(TO_GB(1));
	Logger::init();

	appTime.init();
	window.init();
	gf3dDevice.init(&window);
	renderer.init(&window, &gf3dDevice);
	materialSystem.init(&gf3dDevice, renderer.getSwapchainRenderPass());
	textureSystem.init(&gf3dDevice, 65536);
	textureSystem.getTexture("statue");
	initScene();
	LOGGER_INFO("Initialized engine successfully");
}

void Application::update()
{
	isDonePlaying = window.windowClosed() || glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE);

	const f32 camSpeed = 5.f;
	const f32 camRotSpeed = 50.f;
	gf3d::vec3 camRotation;

	if (glfwGetKey(window.getWindow(), GLFW_KEY_W)) cam.transform.position += cam.transform.forward() * camSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_S)) cam.transform.position -= cam.transform.forward() * camSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_A)) cam.transform.position -= cam.transform.right() * camSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_D)) cam.transform.position += cam.transform.right() * camSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_Q)) cam.transform.position += cam.transform.up() * camSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_E)) cam.transform.position -= cam.transform.up() * camSpeed * appTime.deltaTime();

	if (glfwGetKey(window.getWindow(), GLFW_KEY_RIGHT)) camRotation.y += camRotSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_LEFT)) camRotation.y -= camRotSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_UP)) camRotation.x += camRotSpeed * appTime.deltaTime();
	if (glfwGetKey(window.getWindow(), GLFW_KEY_DOWN)) camRotation.x -= camRotSpeed * appTime.deltaTime();

	if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE)) {
		// Reset camera position/rotation
		cam.transform.position = gf3d::vec3::zero();
		cam.transform.rotation = gf3d::quaternion::identity();
	}

	cam.transform.rotate(camRotation);

	f32 aspect = window.getAspectRatio();
	//cam.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 10);
	cam.setPerspectiveProjection(aspect, 0.1f, 100.f);
	//cam.setViewDirection(gf3d::vec3(0.f), gf3d::vec3(0.5f, 0.f, 1.f));
	//cam.setViewTarget(gf3d::vec3(-1.f, -2.f, 2.f), gf3d::vec3(0.f, 0.f, 2.5f));
	cam.setView(cam.transform.position, cam.transform.rotation);
	for (auto& gameObject : gameObjects) {
		//gameObject.transform.rotate(0.f, 0.f, 100.f * appTime.deltaTime());
		auto finalTransform = gameObject.transform.mat4() * cam.getViewProjectionMatrix();
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
	materialSystem.destroy();
	textureSystem.cleanup();
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
	  {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	  {{.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	  {{.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	  {{.5f, -.5f, -.5f}, {.9f, .9f, .9f}},

	  // right face (red)
	  {{.5f, -.5f, -.5f}, {1.0f, .0f, .0f}},
	  {{.5f, .5f, -.5f}, {1.0f, .0f, .0f}},
	  {{.5f, .5f, .5f}, {1.0f, .0f, .0f}},
	  {{.5f, -.5f, -.5f}, {1.0f, .0f, .0f}},
	  {{.5f, .5f, .5f}, {1.0f, .0f, .0f}},
	  {{.5f, -.5f, .5f}, {1.0f, .0f, .0f}},

	  // top face (yellow)
	  {{-.5f, .5f, -.5f}, {1.0f, 1.0f, .0f}},
	  {{-.5f, .5f, .5f}, {1.0f, 1.0f, .0f}},
	  {{.5f, .5f, .5f}, {1.0f, 1.0f, .0f}},
	  {{-.5f, .5f, -.5f}, {1.0f, 1.0f, .0f}},
	  {{.5f, .5f, .5f}, {1.0f, 1.0f, .0f}},
	  {{.5f, .5f, -.5f}, {1.0f, 1.0f, .0f}},

	  // left face (cyan)
	  {{-.5f, -.5f, -.5f}, {.0f, 1.0f, 1.0f}},
	  {{-.5f, -.5f, .5f}, {.0f, 1.0f, 1.0f}},
	  {{-.5f, .5f, .5f}, {.0f, 1.0f, 1.0f}},
	  {{-.5f, -.5f, -.5f}, {.0f, 1.0f, 1.0f}},
	  {{-.5f, .5f, .5f}, {.0f, 1.0f, 1.0f}},
	  {{-.5f, .5f, -.5f}, {.0f, 1.0f, 1.0f}},

	  // back face (blue)
	  {{-.5f, -.5f, .5f}, {.0f, .0f, 1.0f}},
	  {{.5f, .5f, .5f}, {.0f, .0f, 1.0f}},
	  {{-.5f, .5f, .5f}, {.0f, .0f, 1.0f}},
	  {{-.5f, -.5f, .5f}, {.0f, .0f, 1.0f}},
	  {{.5f, -.5f, .5f}, {.0f, .0f, 1.0f}},
	  {{.5f, .5f, .5f}, {.0f, .0f, 1.0f}},

	  // bottom face (green)
	  {{-.5f, -.5f, -.5f}, {.0f, 1.0f, .0f}},
	  {{.5f, -.5f, .5f}, {.0f, 1.0f, .0f}},
	  {{-.5f, -.5f, .5f}, {.0f, 1.0f, .0f}},
	  {{-.5f, -.5f, -.5f}, {.0f, 1.0f, .0f}},
	  {{.5f, -.5f, -.5f}, {.0f, 1.0f, .0f}},
	  {{.5f, -.5f, .5f}, {.0f, 1.0f, .0f}},
	  
	};
	newObj.mesh.allocateMesh(gf3dDevice);
	newObj.color = { 0.1f, 0, 1, 1 };

	newObj.material = materialSystem.create(ASSETS_PATH "shaders/test.shader");

	//newObj.material->pushUpdate("color", &newObj.color);

	//newObj.transform.rotate(0.f, -60.f, 0.f);
	newObj.transform.position.z = 1.5f;
	gameObjects.push_back(std::move(newObj));
}

void Application::destroyGameObjects()
{
	auto allocator = gf3dDevice.GetAllocator();

	for (auto& gameobject : gameObjects) {
		gameobject.mesh.destroy(allocator);
	}
}
