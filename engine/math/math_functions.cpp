
#include "math_functions.hpp"
#include "math_constants.hpp"
#include <cmath>

namespace gf3d
{

	float sqrt(f32 x)
	{
		return std::sqrtf(x);
	}

	float abs(f32 x)
	{
		return std::fabsf(x);
	}

	float min(f32 x, f32 y)
	{
		return std::fminf(x, y);
	}

	float max(f32 x, f32 y)
	{
		return std::fmaxf(x, y);
	}

	bool fcmp(f32 x, f32 y)
	{
		return (abs(x - y) <= float_epsilon() * max(1.0f, max(abs(x), abs(y))));
	}

} // namespace gf3d