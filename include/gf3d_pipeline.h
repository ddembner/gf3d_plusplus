#pragma once
#include "gf3d_swapchain.h"
#include <glm/mat4x4.hpp>

struct PushConstantData
{
	glm::mat4 transform;
	glm::vec4 color;
};

class Pipeline
{
public:
	Pipeline();
	~Pipeline();
	void loadPipeline(Gf3dDevice& gf3dDevice, VkRenderPass renderpass, const std::string& vertPath, const std::string& fragPath);
	void destroyPipeline(VkDevice device);
	VkPipeline getGraphicsPipeline() const { return pipeline; }
	VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
private:
	VkShaderModule loadShaderModule(VkDevice device, const std::string& shaderPath);
	void createPipelineLayout(VkDevice device);
	void createGraphicsPipeline(Gf3dDevice& gf3dDevice, VkRenderPass renderpass);
private:
	VkPipeline pipeline;
	VkShaderModule vertModule;
	VkShaderModule fragModule;
	VkPipelineLayout pipelineLayout;
	std::string vertPath;
	std::string fragPath;
};