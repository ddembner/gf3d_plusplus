#include "core/gf3d_logger.h"
#include "texture_system.h"
#include "core/gf3d_memory.h"
#include "renderer/vulkan_image.h"
#include "renderer/vulkan_functions.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

template<> gf3d::TextureSystem* Singleton<gf3d::TextureSystem>::instance = nullptr;

namespace gf3d
{

	struct VulkanTextureData
	{
		VulkanImage image;
		VkSampler sampler = nullptr;
	};

	void TextureSystem::init(Gf3dDevice* device, u64 maxTextures)
	{
		if (!registeredTextures) {
			gf3dDevice = device;
			maxCount = maxTextures;
			registeredTextures = reinterpret_cast<Texture*>(malloc(sizeof(Texture) * maxTextures, memory_type::eTexture));
			textureReferences.reserve(maxTextures);

			for (u32 i = 0; i < maxTextures; i++) {
				new (registeredTextures + i) Texture();
			}
		}
	}

	void TextureSystem::cleanup()
	{
		vkDeviceWaitIdle(gf3dDevice->GetDevice());
		if (registeredTextures) {
			for (u64 i = 0; i < maxCount; i++) {
				destroyTexture(&registeredTextures[i]);
			}
			free(registeredTextures, sizeof(Texture) * maxCount, memory_type::eTexture);
		}
	}

	Texture* TextureSystem::getTexture(const char* name, bool autoRelease)
	{
		TextureReference ref;
		if (textureReferences.find(name) != textureReferences.end()) {
			ref.referenceCount = textureReferences[name].referenceCount += 1;
			ref.handle = textureReferences[name].handle;
			LOGGER_TRACE("Texture '{}' already exists, ref count increased to: {}", name, ref.referenceCount);
			return &registeredTextures[ref.handle];
		}

		if (ref.referenceCount == 0) {
			ref.autoRelease = autoRelease;
		}

		ref.referenceCount++;

		if (ref.handle == INVALID_ID) {
			// Create a new texture

			Texture* newTexture = nullptr;
			// Find open texture slot
			for (u32 i = 0; i < maxCount; i++) {
				if (registeredTextures[i].id == INVALID_ID) {
					newTexture = &registeredTextures[i];
					ref.handle = i;
					break;
				}
			}

			if (!newTexture || ref.handle == INVALID_ID) {
				LOGGER_ERROR("Maximum textures are in use. Change max textures for more.");
				return nullptr;
			}

			// Load new texture
			if (!loadTexture(name, newTexture)) {
				LOGGER_ERROR("Failed to load texture: {}", name);
				return nullptr;
			}

			newTexture->id = ref.handle;
			textureReferences[name] = ref;
			return &registeredTextures[newTexture->id];
		}

		LOGGER_ERROR("Failed to get texture: {}", name);
		return nullptr; 
	}

	void TextureSystem::releaseTexture(const char* name)
	{
		auto it = textureReferences.find(name);
		if (it == textureReferences.end()) {
			LOGGER_WARN("Tried to delete a non-existent texture: {}", name);
			return;
		}

		TextureReference& ref = it->second;
		ref.referenceCount--;
		
		if (ref.referenceCount == 0 && ref.autoRelease) {
			Texture* texture = &registeredTextures[ref.handle];

			destroyTextureWaitIdle(texture);
			textureReferences.erase(it);

			ref.autoRelease = false;
			ref.handle = INVALID_ID;

			LOGGER_TRACE("Released texture {}. Texture destroyed because ref count=0 and auto_release=true", name);
			return;
		}
		LOGGER_TRACE("Released texture {}. ref count={}, auto_release={}", name, ref.referenceCount, ref.autoRelease);
	}

	bool TextureSystem::loadTexture(const char* name, Texture* texture)
	{
		TextureInfo info = {};
		if (!getTextureInfo(name, &info)) {
			LOGGER_ERROR("Failed to get info for texture: {}", name);
			return false;
		}

		Texture newTexture = Texture();
		newTexture.width = info.width;
		newTexture.height = info.height;
		newTexture.channelCount = info.channelCount;
		
		//Check if texture has transparency
		u64 totalSize = newTexture.width * newTexture.height * newTexture.channelCount;
		for (u64 i = 0; i < totalSize; i += newTexture.channelCount) {
			u8 alpha = info.data[i + 3];
			if (alpha < 255) {
				newTexture.hasTransparency = true;
				break;
			}
		}

		newTexture.setResourceInfo(info.name, info.filePath, resource_type::eTexture);

		u32 currentGeneration = texture->generation;
		texture->generation = INVALID_ID;

		createAndLoadToGPU(info.data, &newTexture);

		Texture oldTexture = *texture;
		*texture = newTexture;
		destroyTextureWaitIdle(&oldTexture);

		stbi_image_free(info.data);

		if (currentGeneration == INVALID_ID)
			texture->generation = 0;
		else
			texture->generation = currentGeneration++;

		return true;
	}

	bool TextureSystem::getTextureInfo(const char* name, TextureInfo* textureInfo)
	{
		bool foundFile = false;
		std::string fullPath;
		fullPath.reserve(512);
		for (const auto& file : std::filesystem::recursive_directory_iterator(ASSETS_PATH)) {
			if (file.path().stem().compare(name) == 0) {
				foundFile = true;
				fullPath = std::move(file.path().u8string());
			}
		}

		if (!foundFile) {
			LOGGER_ERROR("Could not find file with name: {}", name);
			return false;
		}

		i32 width, height, comp;
		const i32 channelsRequired = STBI_rgb_alpha;
		u8* data = stbi_load(fullPath.c_str(), &width, &height, &comp, channelsRequired);

		if (!data) {
			LOGGER_ERROR("Failed to load image: {}", name);
			return false;
		}

		textureInfo->width = width;
		textureInfo->height = height;
		textureInfo->channelCount = channelsRequired;
		textureInfo->data = data;
		textureInfo->name = name;
		textureInfo->filePath = std::move(fullPath);

		return true;
	}

	void TextureSystem::createAndLoadToGPU(const u8* pixels, Texture* texture)
	{
		texture->vulkanData = malloc(sizeof(VulkanTextureData), memory_type::eTexture);
		VulkanTextureData* textureData = reinterpret_cast<VulkanTextureData*>(texture->vulkanData);
		textureData = new(textureData) VulkanTextureData();
		const VkDeviceSize imageSize = texture->width * texture->height * texture->channelCount;

		auto device = gf3dDevice->GetDevice();
		auto allocator = gf3dDevice->GetAllocator();

		VulkanBuffer stagingBuffer = createAllocatedBuffer(
			allocator,
			imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VMA_MEMORY_USAGE_CPU_ONLY);

		void* data;
		VK_CHECK(vmaMapMemory(allocator, stagingBuffer.allocation, &data));
		memcpy(data, pixels, imageSize);
		vmaUnmapMemory(allocator, stagingBuffer.allocation);

		const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

		textureData->image = VulkanImage(
			gf3dDevice,
			VK_IMAGE_TYPE_2D,
			texture->width,
			texture->height,
			format,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			VMA_MEMORY_USAGE_GPU_ONLY);

		textureData->image.createImageView(gf3dDevice, format, VK_IMAGE_ASPECT_COLOR_BIT);

		VkCommandBuffer tempCmd;
		VkCommandPool pool = gf3dDevice->GetCommandPool();
		VkQueue queue = gf3dDevice->GetGraphicsQueue();

		vulkanCommandBufferSingleUseBegin(gf3dDevice, pool, &tempCmd);

		textureData->image.transitionLayout(
			gf3dDevice,
			tempCmd,
			format,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		textureData->image.copyImageFromBuffer(gf3dDevice, stagingBuffer.buffer, tempCmd);

		textureData->image.transitionLayout(
			gf3dDevice,
			tempCmd,
			format,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vulkanCommandBufferSingleUseEnd(gf3dDevice, pool, &tempCmd, queue);

		vmaDestroyBuffer(allocator, stagingBuffer.buffer, stagingBuffer.allocation);

		VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &textureData->sampler));

		texture->generation++;
	}

	void TextureSystem::destroyTexture(Texture* texture)
	{
		VulkanTextureData* data = reinterpret_cast<VulkanTextureData*>(texture->vulkanData);
		if (data) {
			data->image.destroy(gf3dDevice);
			if (data->sampler) {
				vkDestroySampler(gf3dDevice->GetDevice(), data->sampler, nullptr);
				data->sampler = nullptr;
			}

			free(data, sizeof(VulkanTextureData), memory_type::eTexture);
		}
		*texture = Texture();
	}

	void TextureSystem::destroyTextureWaitIdle(Texture* texture)
	{
		vkDeviceWaitIdle(gf3dDevice->GetDevice());
		destroyTexture(texture);
	}

} // namespace gf3d