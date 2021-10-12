#include "gf3d_camera.h"
#include "gf3d_logger.h"

void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	projectionMatrix = gf3d::mat4{ 1.0f };
	projectionMatrix[0][0] = 2.f / (right - left);
	projectionMatrix[1][1] = 2.f / (bottom - top);
	projectionMatrix[2][2] = 1.f / (zFar - zNear);
	projectionMatrix[3][0] = -(right + left) / (right - left);
	projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
	projectionMatrix[3][2] = -zNear / (zFar - zNear);
}

void Camera::setPerspectiveProjection(float aspect, float zNear, float zFar)
{
	
	projectionMatrix = gf3d::mat4::perspective(fieldOfView, aspect, zNear, zFar);
}

void Camera::setViewDirection(const gf3d::vec3& position, const gf3d::vec3& direction, const gf3d::vec3& up)
{
	viewMatrix = gf3d::mat4::lookDirection(position, direction, up);
}

void Camera::setViewTarget(const gf3d::vec3& position, const gf3d::vec3& target, const gf3d::vec3& up)
{
	viewMatrix = gf3d::mat4::lookAt(position, target, up);
}

void Camera::OnUpdate()
{
	//viewMatrix = glm::translate(glm::mat4(1.0f), position);
}