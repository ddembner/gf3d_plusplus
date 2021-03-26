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
	createRenderPass(swapchain, device);
	createGraphicsPipeline(device, swapchain);

}

void Pipeline::destroyPipeline(VkDevice device)
{
	vkDestroyShaderModule(device, vertModule, nullptr);
	vkDestroyShaderModule(device, fragModule, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
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

void Pipeline::createRenderPass(const Swapchain& swapchain, VkDevice device)
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapchain.getColorFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependacy = {};
	dependacy.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependacy.dstSubpass = 0;
	dependacy.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.srcAccessMask = 0;
	dependacy.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependacy.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &colorAttachment;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpassDescription;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependacy;

	VK_CHECK(vkCreateRenderPass(device, &createInfo, nullptr, &renderPass));
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

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain.getExtent().width);
	viewport.height = static_cast<float>(swapchain.getExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain.getExtent();

	VkPipelineViewportStateCreateInfo viewportStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

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

	VkDynamicState dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
	dynamicStateInfo.dynamicStateCount = 1;
	dynamicStateInfo.pDynamicStates = &dynamicState;

	VkGraphicsPipelineCreateInfo graphicsInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
	graphicsInfo.renderPass = renderPass;
	graphicsInfo.layout = pipelineLayout;
	graphicsInfo.stageCount = 2;
	graphicsInfo.pStages = shaderStages;
	graphicsInfo.pRasterizationState = &rasterizerInfo;
	graphicsInfo.pInputAssemblyState =  &inputAssembly;
	graphicsInfo.pViewportState = &viewportStateInfo;
	graphicsInfo.pMultisampleState = &multisampleInfo;
	graphicsInfo.pColorBlendState = &blendInfo;
	graphicsInfo.pVertexInputState = &vertexInput;
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

void Material::createMaterial(const Swapchain& swapchain, VkDevice device, const std::string& vertPath, const std::string& fragPath)
{

}
