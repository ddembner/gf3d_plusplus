#include "gf3d_camera.h"
#include "gf3d_logger.h"

void Camera::setOrthographicProjection(f32 left, f32 right, f32 top, f32 bottom, f32 zNear, f32 zFar)
{
	projectionMatrix = gf3d::mat4::orthographic(left, right, top, bottom, zNear, zFar);
}

void Camera::setPerspectiveProjection(f32 aspect, f32 zNear, f32 zFar)
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