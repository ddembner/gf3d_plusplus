#pragma once

#include "defines.hpp"

namespace gf3d
{

	struct mat2
	{
		union
		{
			struct
			{
				f32 _11, _12,
					_21, _22;
			};
			f32 elements[4];
		};

		inline constexpr mat2()
			:	_11(1), _12(0),
				_21(0), _22(1)
		{ }

		inline constexpr mat2(f32 s)
			:	_11(s), _12(0),
				_21(0), _22(s)
		{ }

		inline constexpr mat2(
			f32 n11, f32 n12,
			f32 n21, f32 n22)
			:	_11(n11), _12(n12),
				_21(n21), _22(n22)
		{ }

		inline mat2 operator*(f32 scalar) const
		{
			mat2 result = {};
			for (u64 i = 0; i < 4; i++)
				result.elements[i] = elements[i];
			return result;
		}

		inline mat2 operator*(const mat2& other) const
		{
			const f32 a11 = this->_11;
			const f32 a12 = this->_12;
			const f32 a21 = this->_21;
			const f32 a22 = this->_22;

			const f32 b11 = other._11;
			const f32 b12 = other._12;
			const f32 b21 = other._21;
			const f32 b22 = other._22;

			mat2 result = { };
			result._11 = a11 * b11 + a12 * b21;
			result._12 = a11 * b12 + a12 * b22;
			result._21 = a21 * b11 + a22 * b21;
			result._22 = a21 * b12 + a22 * b22;

			return result;
		}

		// NOTE: indexing starts as 0 and so [0][0] eqauls _11 from the union.
		inline constexpr f32* operator[](u64 index)
		{
			return &elements[index * 2];
		}

		inline static constexpr mat2 zero()
		{
			return { 0 };
		}

		inline static constexpr mat2 identity()
		{
			mat2 result = { 0 };
			result._11 = 1.f;
			result._22 = 1.f;
			return result;
		}

		inline mat2 transpose() const
		{
			mat2 result = { };
			for (i32 i = 0; i < 4; i++) {
				i32 row = i / 2;
				i32 col = i % 2;
				result.elements[i] = this->elements[2 * col + row];
			}
			return result;
		}

		inline f32 determinant() const
		{
			return _11 * _22 - _12 * _21;
		}

		inline mat2 inverse() const
		{
			float det = _11 * _22 - _12 * _21;
			if (fcmp(det, 0.0f)) {
				return mat2();
			}
			mat2 result;
			float i_det = 1.0f / det; 
			result._11 = _22 * i_det;
			result._12 = -_12 * i_det;
			result._21 = -_21 * i_det;
			result._22 = _11 * i_det;
			return result;

		}
	};

} // namespace gf3d