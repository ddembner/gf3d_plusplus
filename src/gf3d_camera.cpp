#include "gf3d_camera.h"

void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	projectionMatrix = glm::ortho(left, right, top, bottom, zNear, zFar);
	projectionMatrix[1][1] *= -1;
}

void Camera::setPerspectiveProjection(float fov, float aspect, float near, float far)
{
	projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
	projectionMatrix[2][1] *= -1;
}

void Camera::OnUpdate()
{
	viewMatrix = glm::translate(glm::mat4(1.0f), position);
}