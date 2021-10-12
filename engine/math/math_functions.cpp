
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

	i32 sign(i32 x)
	{
		return x == 0 ? 0 : 
			x > 0 ? 1 : -1;
	}

	f32 sign(f32 x)
	{
		return x == 0.f ? 0.f :
			x > 0.f ? 1.f : -1.f;
	}

} // namespace gf3d