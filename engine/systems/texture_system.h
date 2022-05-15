#pragma once
#include "defines.hpp"
#include "core/gf3d_singleton.h"
#include "renderer/gf3d_device.h"
#include "resources/gf3d_texture.h"
#include <unordered_map>

namespace gf3d
{
	class TextureSystem : public Singleton<TextureSystem>
	{
	private:
		class TextureInfo;
	public:
		void init(Gf3dDevice* device, u64 maxTextures);
		void cleanup();
		Texture* getTexture(const char* name, bool autoRelease = true);
		void releaseTexture(const char* name);
		inline u64 maxTextures() const noexcept { return maxCount; }
	private:
		bool loadTexture(const char* name, Texture* texture);
		bool getTextureInfo(const char* name, TextureInfo* textureInfo);
		void createAndLoadToGPU(const u8* pixels, Texture* texture);
		void destroyTexture(Texture* texture);
		void destroyTextureWaitIdle(Texture* texture);
	private:
		struct TextureReference
		{
			u64 referenceCount = 0;
			u32 handle = INVALID_ID;
			bool autoRelease = false;
		};

		struct TextureInfo
		{
			u32 width = 0;
			u32 height = 0;
			u32 channelCount = 0;
			u8* data = nullptr;
			const char* name;
			std::string filePath;
		};

		Texture* registeredTextures = nullptr;
		std::unordered_map<std::string, TextureReference> textureReferences;
		u64 maxCount = 0;
		Gf3dDevice* gf3dDevice = nullptr;
	};
}