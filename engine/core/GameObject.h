#pragma once
#include "renderer/gf3d_mesh.h"
#include "renderer/gf3d_material.h"

#include "math/mat4.hpp"
#include "math/quaternion.hpp"

struct Transform
{
	gf3d::vec3 position{};
	gf3d::vec3 scale{ 1.f, 1.f, 1.f };
	gf3d::vec3 eulerAngles{};
    gf3d::quaternion rotation{};

	gf3d::mat4 mat4()
	{
        const float c3 = gf3d::cos(eulerAngles.z);
        const float s3 = gf3d::sin(eulerAngles.z);
        const float c2 = gf3d::cos(eulerAngles.x);
        const float s2 = gf3d::sin(eulerAngles.x);
        const float c1 = gf3d::cos(eulerAngles.y);
        const float s1 = gf3d::sin(eulerAngles.y);
        return gf3d::mat4{
            
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            
            
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            
            position.x, position.y, position.z, 1.0f };
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