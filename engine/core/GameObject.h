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
        const f32 xpow = rotation.x * rotation.x;
        const f32 ypow = rotation.y * rotation.y;
        const f32 zpow = rotation.z * rotation.z;
        const f32 xy = rotation.x * rotation.y;
        const f32 xz = rotation.x * rotation.z;
        const f32 yz = rotation.y * rotation.z;
        const f32 xw = rotation.x * rotation.w;
        const f32 yw = rotation.y * rotation.w;
        const f32 zw = rotation.z * rotation.w;

        return gf3d::mat4{

                scale.x * (1.0f - 2.0f * ypow - 2.0f * zpow),
                scale.x * (2.0f * xy - 2.0f * zw),
                scale.x * (2.0f * xz + 2.0f * yw),
                0.0f,

                scale.y *  (2.0f * xy + 2.0f * zw),
                scale.y * (1.0f - 2.0f * xpow - 2.0f * zpow),
                scale.y * (2.0f * yz - 2.0f * xw),
                0.0f,


                scale.z * (2.0f * xz - 2.0f * yw),
                scale.z * (2.0f * yz + 2.0f * xw),
                scale.z * (1.0f - 2.0f * xpow - 2.0f * ypow),
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

    inline gf3d::vec3 right() const
    {
        return gf3d::vec3(
            1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z),
            2 * (rotation.x * rotation.y + rotation.w * rotation.z),
            2 * (rotation.x * rotation.z - rotation.w * rotation.y)
        );
    }

    inline gf3d::vec3 up() const
    {
        return gf3d::vec3(
            2 * (rotation.x * rotation.y - rotation.w * rotation.z),
            1 - 2 * (rotation.x * rotation.x + rotation.z * rotation.z),
            2 * (rotation.y * rotation.z + rotation.w * rotation.x)
        );
    }

    inline gf3d::vec3 forward() const
    {
        return gf3d::vec3(
            2 * (rotation.x * rotation.z + rotation.w * rotation.y), 
            2 * (rotation.y * rotation.z - rotation.w * rotation.x),
            1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y)
        );
    }
};

class GameObject : NonCopyable
{
public:
	Mesh mesh;
	Material* material;
    Transform transform{};
	gf3d::vec4 color;
};