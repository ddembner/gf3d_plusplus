#include "gf3d_logger.h"
#include "gf3d_camera.h"

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
    const f32 xy = -rotation.x * rotation.y;
    const f32 xz = -rotation.x * rotation.z;
    const f32 yz = -rotation.y * rotation.z;
    const f32 xw = -rotation.x * rotation.w;
    const f32 yw = -rotation.y * rotation.w;
    const f32 zw = -rotation.z * rotation.w;

    const gf3d::vec3 u{ 1.0f - 2.0f * ypow - 2.0f * zpow, 2.0f * xy - 2.0f * zw, 2.0f * xz + 2.0f * yw };
    const gf3d::vec3 v{ 2.0f * xy + 2.0f * zw, 1.0f - 2.0f * xpow - 2.0f * zpow, 2.0f * yz - 2.0f * xw };
    const gf3d::vec3 w{ 2.0f * xz - 2.0f * yw, 2.0f * yz + 2.0f * xw, 1.0f - 2.0f * xpow - 2.0f * ypow };

	viewMatrix._11 = u.x;
    viewMatrix._12 = v.x;
    viewMatrix._13 = w.x;
    viewMatrix._21 = u.y;
    viewMatrix._22 = v.y;
    viewMatrix._23 = w.y;
    viewMatrix._31 = u.z;
    viewMatrix._32 = v.z;
    viewMatrix._33 = w.z;
	viewMatrix._41 = -u.dot(position);
	viewMatrix._42 = -v.dot(position);
	viewMatrix._43 = -w.dot(position);
}

void Camera::OnUpdate()
{
	//viewMatrix = glm::translate(glm::mat4(1.0f), position);
}