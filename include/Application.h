#pragma once
#include "gf3d_device.h"
#include "gf3d_graphics.h"
#include "GameObject.h"
#include "NonCopyable.h"
#include <memory>

class Application : NonCopyable
{
public: 
	Application() = default;
	~Application() { }
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	void run();

private:
	void init();
	void update();
	void render();
	void cleanup();

private:
	bool isDonePlaying = false;
	Gf3dWindow window;
	Gf3dDevice gf3dDevice;
	Gf3dGraphics renderer;
	std::vector<GameObject> gameObjects;
};