#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "gf3d_camera.h"
#include "gf3d_logger.h"

#include <glm/trigonometric.hpp>

void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	projectionMatrix = glm::mat4{ 1.0f };
	projectionMatrix[0][0] = 2.f / (right - left);
	projectionMatrix[1][1] = 2.f / (bottom - top);
	projectionMatrix[2][2] = 1.f / (zFar - zNear);
	projectionMatrix[3][0] = -(right + left) / (right - left);
	projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
	projectionMatrix[3][2] = -zNear / (zFar - zNear);
}

void Camera::setPerspectiveProjection(float aspect, float zNear, float zFar)
{
	
	const float tanHalfFovy = tan(glm::radians(fieldOfView) / 2.f);
	projectionMatrix = glm::mat4{ 0.0f };
	projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
	projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	projectionMatrix[2][2] = zFar / (zFar - zNear);
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][2] = -(zFar * zNear) / (zFar - zNear);
}

void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	const glm::vec3 f(glm::normalize(direction));
	const glm::vec s(normalize(cross(f, up)));
	const glm::vec u(cross(f, s));

	viewMatrix = glm::mat4{ 1.f };
	viewMatrix[0][0] = s.x;
	viewMatrix[1][0] = s.y;
	viewMatrix[2][0] = s.z;
	viewMatrix[0][1] = u.x;
	viewMatrix[1][1] = u.y;
	viewMatrix[2][1] = u.z;
	viewMatrix[0][2] = f.x;
	viewMatrix[1][2] = f.y;
	viewMatrix[2][2] = f.z;
	viewMatrix[3][0] = -glm::dot(s, position);
	viewMatrix[3][1] = -glm::dot(u, position);
	viewMatrix[3][2] = -glm::dot(f, position);
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	setViewDirection(position, target - position, up);
}

void Camera::OnUpdate()
{
	//viewMatrix = glm::translate(glm::mat4(1.0f), position);
}