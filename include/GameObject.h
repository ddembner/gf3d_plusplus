#pragma once
#include "gf3d_mesh.h"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;
private:
	//TODO Components
	Mesh mesh;
};