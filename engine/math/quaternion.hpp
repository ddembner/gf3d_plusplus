#pragma once

#include "defines.hpp"
#include "math_functions.hpp"
#include "trigonometric_functions.hpp"
#include "math_constants.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace gf3d
{

	struct quaternion
	{
		union {
			struct {
				f32 x, y, z, w;
			};
			f32 elements[4];
		};

		inline constexpr quaternion()
			: x(0.f), y(0.f), z(0.f), w(1.f)
		{}

		inline constexpr quaternion(f32 _x, f32 _y, f32 _z, f32 _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		inline quaternion(const vec3& euler)
		{
			vec3 s = vec3(sin(euler.x * 0.5f), sin(euler.y * 0.5f), sin(euler.z * 0.5f));
			vec3 c = vec3(cos(euler.x * 0.5f), cos(euler.y * 0.5f), cos(euler.z * 0.5f));

			x = s.x * c.y * c.z - c.x * s.y * s.z;
			y = c.x * s.y * c.z + s.x * c.y * s.z;
			z = c.x * c.y * s.z - s.x * s.y * c.z;
			w = c.x * c.y * c.z + s.x * s.y * s.z;
		}

		inline static constexpr quaternion identity()
		{
			return quaternion(0, 0, 0, 1);
		}

		inline constexpr f32 dot(const quaternion& other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
		}

		inline quaternion normalized() const
		{
			f32 oneOverLength = 1.f / gf3d::sqrt(dot(*this));
			return quaternion(x * oneOverLength, y * oneOverLength, z * oneOverLength, w * oneOverLength);
		}

		inline void normalize()
		{
			f32 oneOverLength = 1.f / gf3d::sqrt(dot(*this));
			*this = quaternion(x * oneOverLength, y * oneOverLength, z * oneOverLength, w * oneOverLength);
		}

		inline quaternion inverse() const
		{
			return quaternion(-x, -y, -z, w);
		}

		inline vec3 eulerAngles() const
		{
			vec3 result;

			// pitch
			f32 _xp = 2 * (w * x + y * z);
			f32 _yp = w * w - x * x - y * y + z * z;
			result.x = atan(_xp / _yp);

			// yaw
			f32 sinp = 2 * (w * y - z * x);
			if (abs(sinp) >= 1)
				result.y = (pi() / 2) * sign(sinp); // use 90 degrees if out of range
			else
				result.y = asin(sinp);

			// roll
			f32 siny_cosp = 2 * (w * z + x * y);
			f32 cosy_cosp = 1 - 2 * (y * y + z * z);
			result.z = atan(siny_cosp / cosy_cosp);

			return result;
		}

		inline mat3 toMat3() const
		{
			mat3 mat;

			quaternion normal = normalized();

			mat._11 = 1.0f - 2.0f * y * y - 2.0f * z * z;
			mat._12 = 2.0f * x * y - 2.0f * z * w;
			mat._13 = 2.0f * x * z + 2.0f * y * w;

			mat._21 = 2.0f * x * y + 2.0f * z * w;
			mat._22 = 1.0f - 2.0f * x * x - 2.0f * z * z;
			mat._23 = 2.0f * y * z - 2.0f * x * w;

			mat._31 = 2.0f * x * z - 2.0f * y * w;
			mat._32 = 2.0f * y * z + 2.0f * x * w;
			mat._33 = 1.0f - 2.0f * x * x - 2.0f * y * y;

			return mat;
		}

		inline mat4 toMat4() const
		{
			mat4 mat = mat4();

			quaternion normal = normalized();

			mat._11 = 1.0f - 2.0f * y * y - 2.0f * z * z;
			mat._12 = 2.0f * x * y - 2.0f * z * w;
			mat._13 = 2.0f * x * z + 2.0f * y * w;

			mat._21 = 2.0f * x * y + 2.0f * z * w;
			mat._22 = 1.0f - 2.0f * x * x - 2.0f * z * z;
			mat._23 = 2.0f * y * z - 2.0f * x * w;

			mat._31 = 2.0f * x * z - 2.0f * y * w;
			mat._32 = 2.0f * y * z + 2.0f * x * w;
			mat._33 = 1.0f - 2.0f * x * x - 2.0f * y * y;

			return mat;
		}
	};

} // namespace gf3d