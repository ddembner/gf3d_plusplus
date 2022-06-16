#pragma once
#include "gf3d_swapchain.h"
#include "gf3d_shader.h"

class Pipeline
{
public:
	Pipeline(VkDevice device, VkRenderPass renderPass, const char* shaderPath);
	~Pipeline();
	void destroyPipeline(VkDevice device);
	VkPipeline getGraphicsPipeline() const { return pipeline; }
	VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
	void updatePush(const char* name, void* data);
	void submitPush(const VkCommandBuffer& cmd);
private:
	VkShaderModule loadShaderModule(VkDevice device, const std::string& shaderPath);
	void createPipelineLayout(VkDevice device);
	void createGraphicsPipeline(VkDevice device, VkRenderPass renderpass);
private:
	VkPipeline pipeline;
	Shader shader;
	VkPipelineLayout pipelineLayout;
	u32 pushSize;
	u8* pushData;
};