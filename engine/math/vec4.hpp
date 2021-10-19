#pragma once

#include "defines.hpp"
#include "math_functions.hpp"
#include "trigonometric_functions.hpp"
#include "core/gf3d_assert.hpp"

namespace gf3d
{

	struct vec4
	{
		union {
			struct {
				f32 x, y, z, w;
			};
			struct {
				f32 r, g, b, a;
			};
			f32 elements[4];
		};

		inline constexpr vec4()
			: x(0.f), y(0.f), z(0.f), w(0.f)
		{}

		inline constexpr vec4(f32 scalar)
			: x(scalar), y(scalar), z(scalar), w(scalar)
		{}

		inline constexpr vec4(f32 _x, f32 _y, f32 _z, f32 _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		inline constexpr vec4 operator+(const vec4& other) const
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}

		inline constexpr vec4 operator+(f32 scalar) const
		{
			return { x + scalar, y + scalar, z + scalar, w + scalar };
		}

		inline constexpr vec4 operator-(const vec4& other) const
		{
			return { x - other.x, y - other.y, z - other.z, w - other.w };
		}

		inline constexpr vec4 operator-(f32 scalar) const
		{
			return { x - scalar, y - scalar, z - scalar, w - scalar };
		}

		inline constexpr vec4 operator*(const vec4& other) const
		{
			return { x * other.x, y * other.y, z * other.z, w * other.w };
		}

		inline constexpr vec4 operator*(f32 scalar) const
		{
			return { x * scalar, y * scalar, z * scalar, w * scalar };
		}

		inline constexpr vec4 operator/(const vec4& other) const
		{
			return { x / other.x, y / other.y, z / other.z, w / other.w };
		}

		inline constexpr vec4 operator/(f32 scalar) const
		{
			return { x / scalar, y / scalar, z / scalar, w / scalar };
		}

		inline bool operator==(const vec4& other) const
		{
			return fcmp(x, other.x) && fcmp(y, other.y) && fcmp(z, other.z) && fcmp(w, other.w);
		}

		inline bool operator!=(const vec4& other) const
		{
			return !(*this == other);
		}

		f32& operator[](u64 index)
		{
			GFASSERT(index < 4, "Index out of range");
			return elements[index];
		}

		const f32& operator[](u64 index) const
		{
			GFASSERT(index < 4, "Index out of range");
			return elements[index];
		}

		inline static constexpr vec4 zero()
		{
			return { 0.f, 0.f, 0.f, 0.f };
		}

		inline static constexpr vec4 one()
		{
			return { 1.f, 1.f, 1.f, 1.f };
		}

		inline constexpr f32 dot(const vec4& other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
		}

		inline f32 magnitude() const
		{
			return sqrt(dot(*this));
		}

		inline f32 magnitudeSq() const
		{
			return dot(*this);
		}

		inline f32 distance(const vec4& other) const
		{
			vec4 t = *this - other;
			return t.magnitude();
		}

		inline constexpr void normalize()
		{
			*this = *this * (1.f / magnitude());
		}

		inline constexpr vec4 normalized() const
		{
			return *this * (1.f / magnitude());
		}

		inline vec4 project(const vec4& direction) const
		{
			float _dot = dot(direction);
			float _magSq = direction.magnitudeSq();
			return direction * (_dot / _magSq);
		}

		inline vec4 perpendicular(const vec4& direction) const
		{
			return *this - project(direction);
		}

	};

} // namespace gf3d