#ifndef __GF3D__GRAPHICS_H__
#define __GF3D__GRAPHICS_H__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct gf3d_window {
	GLFWwindow* glfw_window;
	int width;
	int height;
};

class Gf3dGraphics
{

private:
	//Vulkan Specifics
	VkInstance instance;
	
	//GLFW Window Specifics
	gf3d_window window;

	//Class Functions
	void initVulkan();
	void initWindow();

public:
	Gf3dGraphics(int width, int height);
	~Gf3dGraphics();
	GLFWwindow* getWindow();
	
};

#endif // !__GF3D__GRAPHICS_H__
