#pragma once
#include "gf3d_swapchain.h"
#include "gf3d_shader.h"
#include <glm/mat4x4.hpp>

struct PushConstantData
{
	glm::mat4 transform;
	glm::vec4 color;
};

class Pipeline
{
public:
	Pipeline(VkDevice device, VkRenderPass renderPass, const std::string& shaderPath);
	~Pipeline();
	void loadPipeline(VkDevice device, VkRenderPass renderpass, const std::string& shaderPath);
	void destroyPipeline(VkDevice device);
	VkPipeline getGraphicsPipeline() const { return pipeline; }
	VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
private:
	VkShaderModule loadShaderModule(VkDevice device, const std::string& shaderPath);
	void createPipelineLayout(VkDevice device);
	void createGraphicsPipeline(VkDevice device, VkRenderPass renderpass);
private:
	VkPipeline pipeline;
	Shader shader;
	VkPipelineLayout pipelineLayout;
};