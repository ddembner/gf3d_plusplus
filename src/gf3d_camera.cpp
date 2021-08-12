#include "gf3d_camera.h"
#include "gf3d_logger.h"

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
	const float tanHalfFovy = tan(glm::radians(fovy) / 2.f);
	projectionMatrix = glm::mat4{ 0.0f };
	projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
	projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	projectionMatrix[2][2] = zFar / (zFar - zNear);
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][2] = -(zFar * zNear) / (zFar - zNear);
}

void Camera::OnUpdate()
{
	//viewMatrix = glm::translate(glm::mat4(1.0f), position);
}