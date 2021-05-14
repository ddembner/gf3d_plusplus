#pragma once
#include "gf3d_graphics.h"
#include "GameObject.h"
#include "gf3d_material_system.h"
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
	void CalculateFPS();
	void initScene();
	void destroyGameObjects();

private:
	bool isDonePlaying = false;
	Gf3dWindow window;
	Gf3dDevice gf3dDevice;
	Gf3dGraphics renderer;
	std::unique_ptr<MaterialSystem> materialSystem;
	std::vector<GameObject> gameObjects;
};