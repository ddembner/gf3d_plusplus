#pragma once
#include "gf3d_time.hpp"
#include "renderer/gf3d_graphics.h"
#include "core/GameObject.h"
#include "renderer/gf3d_material_system.h"
#include "systems/texture_system.h"

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
	MaterialSystem materialSystem;
	gf3d::TextureSystem textureSystem;
	gf3d::vector<GameObject> gameObjects;
	Camera cam;
	Time appTime;
};