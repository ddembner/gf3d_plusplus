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
	VkPipeline getGraphicsPipeline() const { return pipeline.getGraphicsPipeline(); }
	VkPipelineLayout getPipelineLayout() const { return pipeline.getPipelineLayout(); }
private:
	Pipeline pipeline;
};