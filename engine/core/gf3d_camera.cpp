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

void Camera::setView(gf3d::vec3 position, gf3d::quaternion rotation)
{
	const f32 xpow = rotation.x * rotation.x;
    const f32 ypow = rotation.y * rotation.y;
    const f32 zpow = rotation.z * rotation.z;
    const f32 xy = rotation.x * rotation.y;
    const f32 xz = rotation.x * rotation.z;
    const f32 yz = rotation.y * rotation.z;
    const f32 xw = rotation.x * rotation.w;
    const f32 yw = rotation.y * rotation.w;
    const f32 zw = rotation.z * rotation.w;

	viewMatrix._11 = 1.0f - 2.0f * ypow - 2.0f * zpow;
    viewMatrix._12 = 2.0f * xy - 2.0f * zw;
    viewMatrix._13 = 2.0f * xz + 2.0f * yw;
    viewMatrix._21 = 2.0f * xy + 2.0f * zw;
    viewMatrix._22 = 1.0f - 2.0f * xpow - 2.0f * zpow;
    viewMatrix._23 = 2.0f * yz - 2.0f * xw;
    viewMatrix._31 = 2.0f * xz - 2.0f * yw;
    viewMatrix._32 = 2.0f * yz + 2.0f * xw;
    viewMatrix._33 = 1.0f - 2.0f * xpow - 2.0f * ypow;
	viewMatrix._41 = -position.x;
	viewMatrix._42 = -position.y;
	viewMatrix._43 = -position.z;
}

void Camera::OnUpdate()
{
	//viewMatrix = glm::translate(glm::mat4(1.0f), position);
}