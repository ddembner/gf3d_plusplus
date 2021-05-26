#include "gf3d_logger.h"
#include "gf3d_material.h"
#include "gf3d_material_system.h"

Material::Material(VkDevice device, VkRenderPass renderPass, const std::string& shaderPath)
	: pipeline(device, renderPass, shaderPath)
{
	
}

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

void Material::pushUpdate(const std::string& name, void* data)
{
	pipeline.updatePush(name, data);
}

void Material::submitPushConstant(const VkCommandBuffer& cmd)
{
	pipeline.submitPush(cmd);
}
