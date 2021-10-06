#pragma once

namespace gf3d
{  
	inline constexpr float pi()
	{
		return 3.14159265358979323846f;
	}

	inline constexpr float two_pi()
	{
		return 6.28318530717958647692f;
	}

	inline constexpr float half_pi()
	{
		return 1.57079632679489661923f;
	}

	inline constexpr float quarter_pi()
	{
		return 0.785398163397448309616f;
	}

	inline constexpr float one_over_pi()
	{
		return 0.318309886183790671538f;
	}

	inline constexpr float one_over_two_pi()
	{
		return 0.159154943091895335768f;
	}

	inline constexpr float two_over_pi()
	{
		return 0.636619772367581343076f;
	}

	inline constexpr float sqrt_two()
	{
		return 1.41421356237309504880f;
	}

	inline constexpr float sqrt_three()
	{
		return 1.73205080756887729352f;
	}

	inline constexpr float one_over_root_two()
	{
		return 0.707106781186547524401f;	
	}

	inline constexpr float infinity()
	{
		return ((float)(1e+300 * 1e+300));
	}

	inline constexpr float float_epsilon()
	{
		return 1.192092896e-07f;
	}
} // namespace gf3d

