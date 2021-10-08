#pragma once

#include "defines.hpp"

namespace gf3d
{

	struct mat3
	{
		union
		{
			struct
			{
				f32 _11, _12, _13,
					_21, _22, _23,
					_31, _32, _33;
			};
			f32 elements[9];
		};

		inline constexpr mat3()
			:	_11(1), _12(0), _13(0),
				_21(0), _22(1), _23(0),
				_31(0), _32(0), _33(1)
		{ }

		inline constexpr mat3(f32 s)
			:	_11(s), _12(0), _13(0),
				_21(0), _22(s), _23(0),
				_31(0), _32(0), _33(s)
		{ }

		inline constexpr mat3(
			f32 n11, f32 n12, f32 n13,
			f32 n21, f32 n22, f32 n23,
			f32 n31, f32 n32, f32 n33)
			:	_11(n11), _12(n12), _13(n13),
				_21(n21), _22(n22), _23(n23),
				_31(n31), _32(n32), _33(n33)
		{ }

		inline constexpr mat3 operator*(f32 scalar) const
		{
			mat3 result = {};
			for (u64 i = 0; i < 9; i++)
				result.elements[i] = elements[i];
			return result;
		}

		inline mat3 operator*(const mat3& other) const
		{
			const f32 a11 = this->_11;
			const f32 a12 = this->_12;
			const f32 a13 = this->_13;
			const f32 a21 = this->_21;
			const f32 a22 = this->_22;
			const f32 a23 = this->_23;
			const f32 a31 = this->_31;
			const f32 a32 = this->_32;
			const f32 a33 = this->_33;

			const f32 b11 = other._11;
			const f32 b12 = other._12;
			const f32 b13 = other._13;
			const f32 b21 = other._21;
			const f32 b22 = other._22;
			const f32 b23 = other._23;
			const f32 b31 = other._31;
			const f32 b32 = other._32;
			const f32 b33 = other._33;

			mat3 result = { };
			result._11 = a11 * b11 + a12 * b21 + a13 * b31;
			result._12 = a11 * b12 + a12 * b22 + a13 * b32;
			result._13 = a11 * b13 + a12 * b23 + a13 * b33;
			result._21 = a21 * b11 + a22 * b21 + a23 * b31;
			result._22 = a21 * b12 + a22 * b22 + a23 * b32;
			result._23 = a21 * b13 + a22 * b23 + a23 * b33;
			result._31 = a31 * b11 + a32 * b21 + a33 * b31;
			result._32 = a31 * b12 + a32 * b22 + a33 * b32;
			result._33 = a31 * b13 + a32 * b23 + a33 * b33;
			return result;
		}

		// NOTE: indexing starts as 0 and so [0][0] eqauls _11 from the union.
		inline constexpr f32* operator[](u64 index)
		{
			return &elements[index * 3];
		}

		inline static constexpr mat3 zero()
		{
			return { 0 };
		}

		inline static constexpr mat3 identity()
		{
			mat3 result = { 0 };
			result._11 = 1.f;
			result._22 = 1.f;
			result._33 = 1.f;
			return result;
		}

		inline mat3 transpose() const
		{
			mat3 result = { };
			for (i32 i = 0; i < 9; i++) {
				i32 row = i / 3;
				i32 col = i % 3;
				result.elements[i] = this->elements[3 * col + row];
			}
			return result;
		}

		inline f32 determinant() const
		{
			return _11 * (_22 * _33 - _23 * _32) - _12 * (_21 * _33 - _23 * _31) + _13 * (_21 * _32 - _22 * _31);
		}
	};

} // namespace gf3d