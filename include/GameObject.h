#pragma once
#include "gf3d_mesh.h"
#include "NonCopyable.h"

class GameObject : NonCopyable
{
public:
	Mesh mesh;
	glm::mat4 transform{ 1.f };
	glm::vec4 color;
};