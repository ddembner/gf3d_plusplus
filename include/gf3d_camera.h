#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	void setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
	const glm::mat4& getViewMatrix() const { return viewMatrix; }
	const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
	glm::mat4 getViewProjectionMatrix() const { return projectionMatrix * viewMatrix; }
	void OnUpdate();
public:
	glm::vec3 position;
private:
	glm::mat4 viewMatrix{ 1.f };
	glm::mat4 projectionMatrix{ 1.f };
};