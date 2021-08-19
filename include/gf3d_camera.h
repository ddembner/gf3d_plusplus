#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class Camera
{
public:
	void setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	void setPerspectiveProjection(float aspect, float zNear, float zFar);
	void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
	void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
	void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
	const glm::mat4& getViewMatrix() const { return viewMatrix; }
	const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
	glm::mat4 getViewProjectionMatrix() const { return projectionMatrix * viewMatrix; }
	void OnUpdate();
public:
	//glm::vec3 position{};
	float fieldOfView = 60.f;
private:
	glm::mat4 viewMatrix{ 1.f };
	glm::mat4 projectionMatrix{ 1.f };
};