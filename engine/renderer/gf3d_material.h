#pragma once
#include "gf3d_pipeline.h"

class MaterialSystem;

class Material
{
public:
	friend class MaterialSystem;
	~Material() { }
	Material(VkDevice device, VkRenderPass renderPass, const char* shaderPath);
	Material* Create(const std::string& vertPath, const std::string& fragPath);
	void freeMaterial(VkDevice device);
	void bindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
	void pushUpdate(const char* name, void* data);
	void submitPushConstant(const VkCommandBuffer& cmd);
private:
	Pipeline pipeline;
};