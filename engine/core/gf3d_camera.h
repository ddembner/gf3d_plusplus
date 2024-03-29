#pragma once
#include "math/mat4.hpp"
#include "math/quaternion.hpp"
#include "GameObject.h"

class Camera
{
public:
	void setOrthographicProjection(f32 left, f32 right, f32 top, f32 bottom, f32 zNear, f32 zFar);
	void setPerspectiveProjection(f32 aspect, f32 zNear, f32 zFar);
	void setViewDirection(const gf3d::vec3& position, const gf3d::vec3& direction, const gf3d::vec3& up = gf3d::vec3{ 0.f, 1.f, 0.f });
	void setViewTarget(const gf3d::vec3& position, const gf3d::vec3& target, const gf3d::vec3& up = gf3d::vec3{ 0.f, 1.f, 0.f });
	void setView(gf3d::vec3 position, gf3d::quaternion rotation);
	const gf3d::mat4& getViewMatrix() const { return viewMatrix; }
	const gf3d::mat4& getProjectionMatrix() const { return projectionMatrix; }
	gf3d::mat4 getViewProjectionMatrix() const { return viewMatrix * projectionMatrix; }
	void OnUpdate();
public:
	Transform transform{};

	f32 fieldOfView = 60.f;
private:
	gf3d::mat4 viewMatrix{ 1.f };
	gf3d::mat4 projectionMatrix{ 1.f };
};