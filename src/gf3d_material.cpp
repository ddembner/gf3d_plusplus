#include "gf3d_material.h"
#include "gf3d_material_system.h"

Material* Material::Create(const std::string& vertPath, const std::string& fragPath)
{
	//Check if material already exists
	return nullptr;
}

void Material::freeMaterial(VkDevice device)
{
	pipeline.destroyPipeline(device);
}

void Material::bindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint pipelineBindPoint)
{
	vkCmdBindPipeline(cmd, pipelineBindPoint, pipeline.getGraphicsPipeline());
}

void Material::submitPushConstantData(VkCommandBuffer cmd, VkShaderStageFlags stages, uint32_t size, const PushConstantData* data, uint32_t offset)
{
	vkCmdPushConstants(cmd, pipeline.getPipelineLayout(), stages, offset, size, data);
}
