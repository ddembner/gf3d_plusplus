#pragma once

#include "defines.hpp"

namespace gf3d
{

	float sqrt(f32 x);
	
	float abs(f32 x);
	
	float min(f32 x, f32 y);
	
	float max(f32 x, f32 y);

	bool fcmp(f32 x, f32 y);

	i32 sign(i32 x);

	f32 sign(f32 x);

	f32 mod(f32 x, f32 y);
} // namespace gf3d