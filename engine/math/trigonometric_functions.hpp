#pragma once

#include "defines.hpp"

namespace gf3d
{
	float sin(f32 x);
	
	float cos(f32 x);
	
	float tan(f32 x);
	
	float asin(f32 x);
	
	float acos(f32 x);
	
	float atan(f32 x);

	float atan(f32 y, f32 x);

	inline constexpr float degrees(float radians)
	{
		return radians * 57.2957795130823208768f;
	}

	inline constexpr float radians(float degrees)
	{
		return degrees * 0.01745329251994329577f;
	}

} // namespace gf3d