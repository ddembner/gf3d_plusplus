#pragma once

#include "defines.hpp"
#include "core/gf3d_assert.hpp"

namespace gf3d
{

	struct mat4
	{
		union
		{
			struct
			{
				f32 _11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44;
			};
			f32 elements[16];
		};

		inline constexpr mat4()
			:	_11(1), _12(0), _13(0), _14(0),
				_21(0), _22(1), _23(0), _24(0),
				_31(0), _32(0), _33(1), _34(0),
				_41(0), _42(0), _43(0), _44(1)
		{ }

		inline constexpr mat4(f32 s)
			:	_11(s), _12(0), _13(0), _14(0),
				_21(0), _22(s), _23(0), _24(0),
				_31(0), _32(0), _33(s), _34(0),
				_41(0), _42(0), _43(0), _44(s)
		{ }

		inline constexpr mat4(
			f32 n11, f32 n12, f32 n13, f32 n14,
			f32 n21, f32 n22, f32 n23, f32 n24,
			f32 n31, f32 n32, f32 n33, f32 n34,
			f32 n41, f32 n42, f32 n43, f32 n44)
			:	_11(n11), _12(n12), _13(n13), _14(n14),
				_21(n21), _22(n22), _23(n23), _24(n24),
				_31(n31), _32(n32), _33(n33), _34(n34),
				_41(n41), _42(n42), _43(n43), _44(n44)
		{ }

		inline constexpr mat4 operator*(f32 scalar) const
		{
			mat4 result = {};
			for (u64 i = 0; i < 16; i++)
				result.elements[i] = elements[i];
			return result;
		}

		inline mat4 operator*(const mat4& other) const
		{				

			mat4 result = { };
			const f32* m1_ptr = this->elements;
			const f32* m2_ptr = other.elements;
			f32* dst_ptr = result.elements;

			for (i32 i = 0; i < 4; ++i) {
				for (i32 j = 0; j < 4; ++j) {
					*dst_ptr =
						m1_ptr[0] * m2_ptr[0 + j] +
						m1_ptr[1] * m2_ptr[4 + j] +
						m1_ptr[2] * m2_ptr[8 + j] +
						m1_ptr[3] * m2_ptr[12 + j];
					dst_ptr++;
				}
				m1_ptr += 4;
			}
			return result;
		}

		// NOTE: indexing starts as 0 and so [0][0] eqauls _11 from the union.
		inline constexpr f32* operator[](u64 index)
		{
			return &elements[index * 4];
		}

		inline static constexpr mat4 zero()
		{
			return { 0 };
		}

		inline static constexpr mat4 identity()
		{
			mat4 result = { 0 };
			result._11 = 1.f;
			result._22 = 1.f;
			result._33 = 1.f;
			result._44 = 1.f;
			return result;
		}

		inline mat4 transpose() const
		{
			mat4 result = { };
			for (i32 i = 0; i < 16; i++) {
				i32 row = i / 4;
				i32 col = i % 4;
				result.elements[i] = this->elements[4 * col + row];
			}
			return result;
		}

		inline f32 determinant() const
		{
			float source2 = _12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)); 
			float source1 = _11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)); //good
			float source3 = _13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)); 
			float source4 = _14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));

			return source1 - source2 + source3 - source4;
		}

		inline mat4 inverse() const
		{
			const f32* m = this->elements;

			f32 t0 = m[10] * m[15];
			f32 t1 = m[14] * m[11];
			f32 t2 = m[6] * m[15];
			f32 t3 = m[14] * m[7];
			f32 t4 = m[6] * m[11];
			f32 t5 = m[10] * m[7];
			f32 t6 = m[2] * m[15];
			f32 t7 = m[14] * m[3];
			f32 t8 = m[2] * m[11];
			f32 t9 = m[10] * m[3];
			f32 t10 = m[2] * m[7];
			f32 t11 = m[6] * m[3];
			f32 t12 = m[8] * m[13];
			f32 t13 = m[12] * m[9];
			f32 t14 = m[4] * m[13];
			f32 t15 = m[12] * m[5];
			f32 t16 = m[4] * m[9];
			f32 t17 = m[8] * m[5];
			f32 t18 = m[0] * m[13];
			f32 t19 = m[12] * m[1];
			f32 t20 = m[0] * m[9];
			f32 t21 = m[8] * m[1];
			f32 t22 = m[0] * m[5];
			f32 t23 = m[4] * m[1];

			mat4 result;
			f32* o = result.elements;

			o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
			o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
			o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
			o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

			f32 d = (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

			GFASSERT(d != 0.f, "mat4 inverse has determinate of 0");

			d = 1.0f / d;

			o[0] = d * o[0];
			o[1] = d * o[1];
			o[2] = d * o[2];
			o[3] = d * o[3];
			o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
			o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
			o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
			o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
			o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
			o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
			o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
			o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
			o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
			o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
			o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
			o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

			return result;
		}
	};

} // namespace gf3d