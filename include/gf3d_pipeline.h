#pragma once
#include "gf3d_swapchain.h"
#include <string>

class Pipeline
{
public:
	Pipeline();
	~Pipeline();
	void loadPipeline(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath);
	void destroyPipeline(VkDevice device);
	VkPipeline getGraphicsPipeline() const { return pipeline; }
	VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
private:
	VkShaderModule loadShaderModule(VkDevice device, const std::string& shaderPath);
	void createPipelineLayout(VkDevice device);
	void createGraphicsPipeline(VkDevice device, Swapchain swapchain);
private:
	VkPipeline pipeline;
	VkShaderModule vertModule;
	VkShaderModule fragModule;
	VkPipelineLayout pipelineLayout;
	std::string vertPath;
	std::string fragPath;
};

class Material 
{
public:
	Material();
	Material(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath);
	~Material();
	void freeMaterial(VkDevice device);
	VkPipeline getGraphicsPipeline() const { return pipeline.getGraphicsPipeline(); }
	VkPipelineLayout getPipelineLayout() const { return pipeline.getPipelineLayout(); }
private:
	Pipeline pipeline;
};