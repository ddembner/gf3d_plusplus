#pragma once
#include "gf3d_resource.h"

namespace gf3d
{
	class Texture : public Resource
	{
	public:
		Texture()
		{
			resourceType = resource_type::eTexture;
		}

	public:
		u32 id = INVALID_ID;
		u32 generation = INVALID_ID;
		u32 width = 0;
		u32 height = 0;
		u8 channelCount = 0;
		bool hasTransparency = false;
		void* vulkanData = nullptr;
	};
} // namespace gf3d