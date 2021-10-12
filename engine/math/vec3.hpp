#pragma once

#include "defines.hpp"
#include "math_functions.hpp"
#include "trigonometric_functions.hpp"
#include "core/gf3d_assert.hpp"

namespace gf3d
{

	struct vec3
	{
		union {
			struct {
				f32 x, y, z;
			};
			struct {
				f32 r, g, b;
			};
			f32 elements[3];
		};

		inline constexpr vec3()
			: x(0.f), y(0.f), z(0.f)
		{}

		inline constexpr vec3(f32 scalar)
			: x(scalar), y(scalar), z(scalar)
		{}

		inline constexpr vec3(f32 _x, f32 _y, f32 _z)
			: x(_x), y(_y), z(_z)
		{}

		inline constexpr vec3 operator+(const vec3& other) const
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		inline constexpr vec3 operator+(f32 scalar) const
		{
			return { x + scalar, y + scalar, z + scalar };
		}

		inline constexpr vec3 operator-(const vec3& other) const
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		inline constexpr vec3 operator-(f32 scalar) const
		{
			return { x - scalar, y - scalar, z - scalar };
		}

		inline constexpr vec3 operator*(const vec3& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		inline constexpr vec3 operator*(f32 scalar) const
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		inline constexpr vec3 operator/(const vec3& other) const
		{
			return { x / other.x, y / other.y, z / other.z };
		}

		inline constexpr vec3 operator/(f32 scalar) const
		{
			return { x / scalar, y / scalar, z / scalar };
		}

		inline bool operator==(const vec3& other) const
		{
			return fcmp(x, other.x) && fcmp(y, other.y) && fcmp(z, other.z);
		}

		inline bool operator!=(const vec3& other) const
		{
			return !(*this == other);
		}

		f32& operator[](u64 index)
		{
			GFASSERT(index < 3, "Index out of range");
			return elements[index];
		}

		const f32& operator[](u64 index) const
		{
			GFASSERT(index < 3, "Index out of range");
			return elements[index];
		}

		inline static constexpr vec3 zero()
		{
			return { 0.f, 0.f, 0.f };
		}

		inline static constexpr vec3 one()
		{
			return { 1.f, 1.f, 1.f };
		}

		inline static constexpr vec3 up()
		{
			return { 0.f, 1.f, 0.f };
		}

		inline static constexpr vec3 down()
		{
			return { 0.f, -1.f, 0.f };
		}

		inline static constexpr vec3 left()
		{
			return { -1.f, 0.f, 0.f };
		}

		inline static constexpr vec3 right()
		{
			return { 1.f, 0.f, 0.f };
		}

		inline static constexpr vec3 forward()
		{
			return { 0.f, 0.f, 1.f };
		}

		inline static constexpr vec3 back()
		{
			return { 0.f, 0.f, -1.f };
		}

		inline constexpr f32 dot(const vec3& other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		inline f32 magnitude() const
		{
			return sqrt(dot(*this));
		}

		inline f32 magnitudeSq() const
		{
			return dot(*this);
		}

		inline f32 distance(const vec3& other) const
		{
			vec3 t = *this - other;
			return t.magnitude();
		}

		inline constexpr void normalize()
		{
			*this = *this * (1.f / magnitude());
		}

		inline constexpr vec3 normalized() const
		{
			return *this * (1.f / magnitude());
		}

		inline constexpr vec3 cross(const vec3& other) const
		{
			vec3 result = vec3();
			result.x = y * other.z - z * other.y;
			result.y = z * other.x - x * other.z;
			result.z = x * other.y - y * other.x;
			return result;
		}

		/// <summary>
		/// Gets angle of this vector and other
		/// </summary>
		/// <param name="other"></param>
		/// <returns>value in radians</returns>
		inline float angle(const vec3& other) const
		{
			float m = sqrt(magnitudeSq() * other.magnitudeSq());
			return acos(dot(other) / m);
		}

		inline vec3 project(const vec3& direction) const
		{
			float _dot = dot(direction);
			float _magSq = direction.magnitudeSq();
			return direction * (_dot / _magSq);
		}

		inline vec3 perpendicular(const vec3& direction) const
		{
			return *this - project(direction);
		}

		inline vec3 reflect(const vec3& normal) const
		{
			float _dot = dot(normal);
			return *this - normal * (_dot * 2.0f);
		}

	};

} // namespace gf3d