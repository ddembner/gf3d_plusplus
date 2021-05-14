#pragma once
#include "gf3d_pipeline.h"

class MaterialSystem;

class Material
{
public:
	friend class MaterialSystem;
	Material() { }
	~Material() { }
	Material* Create(const std::string& vertPath, const std::string& fragPath);
	void freeMaterial(VkDevice device);
	void bindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
	void submitPushConstantData(VkCommandBuffer cmd, VkShaderStageFlags stages, uint32_t size, const PushConstantData* data, uint32_t offset = 0);
private:
	Pipeline pipeline;
};