#include "hello_triangle.h"
#include "gf3d_graphics.h"

#if NDEBUG
const bool enableValidation = false;
#else
const bool enableValidation = true;
#endif

void RunTriangleApp() {

	HelloTriangleApplication app;

	app.run();
}

/*
* Steps to setting up vulkan
*	1. Initialize Vulkan - Done
*	2. Create Surface
*	3. Select Physical Device
*	4. Create Logical Device and Get Queues
*	5. Create Pools
*	6. Create Render Pass
*	7. Create Swapchain and Get Images
*/


int main() {

	Gf3dGraphics graphics = { 800, 600 };

	try {
		//RunTriangleApp(); //Run Triangle Example
		while (!glfwWindowShouldClose(graphics.getWindow())) {
			glfwPollEvents();
		}
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}