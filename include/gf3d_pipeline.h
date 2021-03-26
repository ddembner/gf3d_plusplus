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
private:
	VkShaderModule loadShaderModule(VkDevice device, const std::string& shaderPath);
	void createPipelineLayout(VkDevice device);
	void createRenderPass(const Swapchain& swapchain, VkDevice device);
	void createGraphicsPipeline(VkDevice device, Swapchain swapchain);
private:
	VkPipeline pipeline;
	VkShaderModule vertModule;
	VkShaderModule fragModule;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
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
private:
	void createMaterial(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath);
private:
	Pipeline pipeline;
};