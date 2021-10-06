#pragma once

#include "defines.hpp"
#include "math_functions.hpp"
#include "trigonometric_functions.hpp"
#include "core/gf3d_assert.hpp"

namespace gf3d
{

	struct vec2
	{
		union {
			struct {
				f32 x, y;
			};
			struct {
				f32 r, g;
			};
			struct {
				f32 u, v;
			};
			f32 elements[2];
		};

		vec2() = default;

		inline constexpr vec2(f32 scalar)
			: x(scalar), y(scalar) 
		{}

		inline constexpr vec2(f32 _x, f32 _y)
			: x(_x), y(_y)
		{}

		inline constexpr vec2 operator+(const vec2& other) const
		{
			return { x + other.x, y + other.y };
		}

		inline constexpr vec2 operator+(f32 scalar) const
		{
			return { x + scalar, y + scalar };
		}

		inline constexpr vec2 operator-(const vec2& other) const
		{
			return { x - other.x, y - other.y };
		}

		inline constexpr vec2 operator-(f32 scalar) const
		{
			return { x - scalar, y - scalar };
		}

		inline constexpr vec2 operator*(const vec2& other) const
		{
			return { x * other.x, y * other.y };
		}

		inline constexpr vec2 operator*(f32 scalar) const
		{
			return { x * scalar, y * scalar };
		}

		inline constexpr vec2 operator/(const vec2& other) const
		{
			return { x / other.x, y / other.y };
		}

		inline constexpr vec2 operator/(f32 scalar) const
		{
			return { x / scalar, y / scalar };
		}

		inline bool operator==(const vec2& other) const
		{
			return fcmp(x, other.x) && fcmp(y, other.y);
		}

		inline bool operator!=(const vec2& other) const
		{
			return !(*this == other);
		}
		
		f32& operator[](u64 index)
		{
			GFASSERT(index < 2, "Index out of range");
			return elements[index];
		}

		const f32& operator[](u64 index) const
		{
			GFASSERT(index < 2, "Index out of range");
			return elements[index];
		}

		inline static constexpr vec2 zero()
		{
			return { 0.f, 0.f };
		}

		inline static constexpr vec2 one()
		{
			return { 1.f, 1.f };
		}

		inline static constexpr vec2 up()
		{
			return { 0.f, 1.f };
		}

		inline static constexpr vec2 down()
		{
			return { 0.f, -1.f };
		}

		inline static constexpr vec2 left()
		{
			return { -1.f, 0.f };
		}

		inline static constexpr vec2 right()
		{
			return { 1.f, 0.f };
		}

		inline constexpr f32 dot(const vec2& other) const 
		{
			return (x * other.x) + (y * other.y);
		}

		inline f32 magnitude() const
		{
			return sqrt(dot(*this));
		}

		inline f32 magnitudeSq() const
		{
			return dot(*this);
		}

		inline f32 distance(const vec2& other) const
		{
			vec2 t = *this - other;
			return t.magnitude();
		}

		inline constexpr void normalize()
		{
			*this = *this * (1.f / magnitude());
		}

		inline constexpr vec2 normalized() const
		{
			return *this * (1.f / magnitude());
		}

		/// <summary>
		/// Gets angle of this vector and other
		/// </summary>
		/// <param name="other"></param>
		/// <returns>value in radians</returns>
		inline float angle(const vec2& other) const
		{
			float m = sqrt(magnitudeSq() * other.magnitudeSq());
			return acos(dot(other) / m);
		}

		inline vec2 project(const vec2& direction) const
		{
			float _dot = dot(direction);
			float _magSq = direction.magnitudeSq();
			return direction * (_dot / _magSq);
		}

		inline vec2 perpendicular(const vec2& direction) const
		{
			return *this - project(direction);
		}

		inline vec2 reflect(const vec2& normal) const
		{
			float _dot = dot(normal);
			return *this - normal * (_dot * 2.0f);
		}

	};

} // namespace gf3d