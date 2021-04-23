#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	const glm::mat4& getViewMatrix() const { return viewMatrix; }
	glm::mat4 getViewProjectionMatrix() const { return projectionMartix * viewMatrix; }
	void OnUpdate();
public:
	glm::vec3 position;
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMartix;
};