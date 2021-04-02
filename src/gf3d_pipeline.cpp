#include "gf3d_pipeline.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "vulkan_functions.h"

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::loadPipeline(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath)
{
	vertModule = loadShaderModule(device, vertPath);
	fragModule = loadShaderModule(device, fragPath);

	createPipelineLayout(device);
	createGraphicsPipeline(device, swapchain);

}

void Pipeline::destroyPipeline(VkDevice device)
{
	vkDestroyShaderModule(device, vertModule, nullptr);
	vkDestroyShaderModule(device, fragModule, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

VkPipeline Pipeline::getGraphicsPipeline() const
{
	return pipeline;
}

VkShaderModule Pipeline::loadShaderModule(VkDevice device, const std::string& shaderPath)
{

	std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		std::cout << "Failed to open file: " << shaderPath << std::endl;
		return VkShaderModule();
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<uint8_t> shaderCode(fileSize);

	file.seekg(0);
	file.read(reinterpret_cast<char*>(shaderCode.data()), fileSize);

	file.close();
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = static_cast<uint32_t>(shaderCode.size());
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
	createInfo.flags = 0;

	VkShaderModule shaderModule;
	VK_CHECK(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule));

	return shaderModule;
}

void Pipeline::createPipelineLayout(VkDevice device)
{
	VkPipelineLayoutCreateInfo createInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	VK_CHECK(vkCreatePipelineLayout(device, &createInfo, nullptr, &pipelineLayout));
}

void Pipeline::createGraphicsPipeline(VkDevice device, Swapchain swapchain)
{
	VkPipelineShaderStageCreateInfo vertexInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
	vertexInfo.module = vertModule;
	vertexInfo.pName = "main";
	vertexInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineShaderStageCreateInfo fragInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
	fragInfo.module = fragModule;
	fragInfo.pName = "main";
	fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexInfo, fragInfo };

	VkPipelineRasterizationStateCreateInfo rasterizerInfo = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
	rasterizerInfo.depthClampEnable = VK_FALSE;
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizerInfo.depthBiasEnable = VK_FALSE;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.scissorCount = 1;
	

	VkPipelineMultisampleStateCreateInfo multisampleInfo = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo blendInfo = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = &colorBlendAttachment;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;

	VkPipelineVertexInputStateCreateInfo vertexInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

	VkDynamicState dynamicState[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.pDynamicStates = dynamicState;

	VkGraphicsPipelineCreateInfo graphicsInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
	graphicsInfo.renderPass = swapchain.getRenderPass();
	graphicsInfo.layout = pipelineLayout;
	graphicsInfo.stageCount = 2;
	graphicsInfo.pStages = shaderStages;
	graphicsInfo.pRasterizationState = &rasterizerInfo;
	graphicsInfo.pInputAssemblyState =  &inputAssembly;
	graphicsInfo.pViewportState = &viewportStateInfo;
	graphicsInfo.pMultisampleState = &multisampleInfo;
	graphicsInfo.pColorBlendState = &blendInfo;
	graphicsInfo.pVertexInputState = &vertexInput;
	graphicsInfo.pDynamicState = &dynamicStateInfo;
	VK_CHECK(vkCreateGraphicsPipelines(device, nullptr, 1, &graphicsInfo, nullptr, &pipeline));
}

Material::Material()
{
}

Material::Material(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath)
{
	pipeline.loadPipeline(swapchain, device, vertPath, fragPath);
}

Material::~Material()
{
}

void Material::freeMaterial(VkDevice device)
{
	pipeline.destroyPipeline(device);
}

VkPipeline Material::getGraphicsPipeline() const
{
	return pipeline.getGraphicsPipeline();
}
