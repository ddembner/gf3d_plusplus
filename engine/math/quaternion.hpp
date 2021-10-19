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

		/// <summary>
		/// construct quaternion with euler angles with ZXY ordering
		/// </summary>
		/// <param name="euler">vec3 euler angles in degrees</param>
		inline quaternion(const vec3& euler)
		{
			vec3 s = vec3(sin(radians(euler.x) * 0.5f), sin(radians(euler.y) * 0.5f), sin(radians(euler.z) * 0.5f));
			vec3 c = vec3(cos(radians(euler.x) * 0.5f), cos(radians(euler.y) * 0.5f), cos(radians(euler.z) * 0.5f));

			x = s.x * c.y * c.z - c.x * s.y * s.z;
			y = c.x * s.y * c.z + s.x * c.y * s.z;
			z = c.x * c.y * s.z + s.x * s.y * c.z;
			w = c.x * c.y * c.z - s.x * s.y * s.z;
		}

		inline constexpr quaternion operator+(const quaternion& other) const
		{
			return {
				x + other.x,
				y + other.y,
				z + other.z,
				w + other.w
			};
		}

		inline constexpr quaternion operator*(const quaternion& other) const
		{
			return {
				w * other.x + x * other.w + y * other.z - z * other.y,
				w * other.y + y * other.w + z * other.x - x * other.z,
				w * other.z + z * other.w + x * other.y - y * other.x,
				w * other.w - x * other.x - y * other.y - z * other.z
			};
		}

		inline constexpr quaternion operator*(const f32& scalar) const
		{
			return {
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar
			};
		}

		inline constexpr quaternion operator/(const f32& scalar) const
		{
			return {
				x / scalar,
				y / scalar,
				z / scalar,
				w / scalar
			};
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

		inline f32 angle(const quaternion& other) const
		{
			quaternion result = inverse() * other;

			return degrees(2.f * acos(result.w));
		}

		inline quaternion lerp(const quaternion& other, f32 t) const
		{
			return quaternion(x * (1 - t) + other.x * t, y * (1 - t) + other.y * t, z * (1 - t) + other.z * t, w * (1 - t) + other.w * t);
		}

		inline quaternion slerp(const quaternion& other, f32 t)
		{
			f32 dt = dot(other);
			quaternion q2 = other;

			if (dt < 0) {
				dt = -dt;
				q2 = q2 * -1.f;
			}

			if (dt < .99995f) {
				f32 angle = acos(dt);
				return ((*this * sin((1.f - t) * angle) + q2 * sin(t * angle)) / sin(angle));
			}

			return lerp(q2, t);
		}

		// ZXY
		inline vec3 eulerAngles() const
		{
			vec3 result;

			// yaw
			f32 sinp = 2 * (y * z + w * x);
			if (abs(sinp) >= 1)
				result.x = (pi() / 2) * sign(sinp); // use 90 degrees if out of range
			else
				result.x = asin(sinp);
			result.x = degrees(result.x);

			// pitch
			f32 _xp = -2 * (x * z - w * y);
			f32 _yp = w * w - x * x - y * y + z * z;
			result.y = degrees(atan(_xp, _yp));

			// roll
			f32 siny_cosp = -2 * (x * y - w * z);
			f32 cosy_cosp = w * w  - x * x + y * y - z * z;
			result.z = degrees(atan(siny_cosp, cosy_cosp));

			return result;
		}

		inline mat3 toMat3() const
		{
			mat3 mat;

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