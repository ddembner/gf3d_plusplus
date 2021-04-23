#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
private:
	glm::mat4 matrix;
};