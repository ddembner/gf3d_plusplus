#include "trigonometric_functions.hpp"

#include <cmath>

namespace gf3d
{

	float sin(f32 x)
	{
		return std::sinf(x);
	}

	float cos(f32 x)
	{
		return std::cosf(x);
	}

	float tan(f32 x)
	{
		return std::tanf(x);
	}

	float asin(f32 x)
	{
		return std::asinf(x);
	}

	float acos(f32 x)
	{
		return std::acosf(x);
	}

	float atan(f32 x)
	{
		return std::atanf(x);
	}

} // namespace gf3d