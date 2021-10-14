#pragma once
#include "renderer/gf3d_mesh.h"
#include "renderer/gf3d_material.h"

#include "math/mat4.hpp"
#include "math/quaternion.hpp"

struct Transform
{
	gf3d::vec3 position{};
	gf3d::vec3 scale{ 1.f, 1.f, 1.f };
    gf3d::quaternion rotation{};

    gf3d::mat4 mat4()
    {
        return gf3d::mat4{

                scale.x * (1.0f - 2.0f * rotation.y * rotation.y - 2.0f * rotation.z * rotation.z),
                scale.x * (2.0f * rotation.x * rotation.y - 2.0f * rotation.z * rotation.w),
                scale.x * (2.0f * rotation.x * rotation.z + 2.0f * rotation.y * rotation.w),
                0.0f,

                scale.y *  (2.0f * rotation.x * rotation.y + 2.0f * rotation.z * rotation.w),
                scale.y * (1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.z * rotation.z),
                scale.y * (2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w),
                0.0f,


                scale.z * (2.0f * rotation.x * rotation.z - 2.0f * rotation.y * rotation.w),
                scale.z * (2.0f * rotation.y * rotation.z + 2.0f * rotation.x * rotation.w),
                scale.z * (1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.y * rotation.y),
                0.0f,

                position.x, position.y, position.z, 1.0f 
        };
    }

    inline void rotate(const gf3d::vec3& euler)
    {
        rotation = rotation * gf3d::quaternion(euler);
    }

    inline void rotate(const f32& x, const f32& y, const f32& z)
    {
        rotation = rotation * gf3d::quaternion(gf3d::vec3(x, y, z));
    }
};

class GameObject : NonCopyable
{
public:
	Mesh mesh;
	Material* material;
    Transform transform{};
	glm::vec4 color;
};