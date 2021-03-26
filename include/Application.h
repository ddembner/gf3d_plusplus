#pragma once
#include "gf3d_graphics.h"

class Application 
{
public: 
	Application() { }
	~Application() { }
	void run();

private:
	void init();
	void update();
	void render();
	void cleanup();

private:
	bool isDonePlaying = false;
	Gf3dGraphics renderer;
};