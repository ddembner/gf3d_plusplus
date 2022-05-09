#include "core/gf3d_logger.h"
#include "gf3d_pipeline.h"
#include "vulkan_functions.h"
#include "gf3d_mesh.h"
#include "core/gf3d_memory.h"

Pipeline::Pipeline(VkDevice device, VkRenderPass renderPass, const char* shaderPath)
	: shader(device, shaderPath)
	, pushSize(shader.getPushDataSize())
	, pushData(reinterpret_cast<u8*>(gf3d::malloc(pushSize, gf3d::memory_type::eDynamic)))
{
	std::memset(pushData, 0, shader.getPushDataSize());
	createPipelineLayout(device);
	createGraphicsPipeline(device,renderPass);
}

Pipeline::~Pipeline()
{
	gf3d::free(pushData, pushSize, gf3d::memory_type::eDynamic);
}

void Pipeline::destroyPipeline(VkDevice device)
{
	shader.destroy();
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

void Pipeline::updatePush(const char* name, void* data)
{
	auto uniform = shader.getUniform(name);

	if (uniform.type == 0) {
		LOGGER_WARN("Uniform \"{}\" was not found", name);
		return;
	}

	std::memcpy(pushData + uniform.offset, data, uniform.size);
}

void Pipeline::submitPush(const VkCommandBuffer& cmd)
{
	for (const auto& pushRange : shader.getPushConstantRanges()) {
		vkCmdPushConstants(cmd, pipelineLayout, pushRange.stageFlags, pushRange.offset, pushRange.size, pushData + pushRange.offset);
	}
}

void Pipeline::createPipelineLayout(VkDevice device)
{
	gf3d::vector<VkPushConstantRange> pushRanges = shader.getPushConstantRanges();

	VkPipelineLayoutCreateInfo createInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	createInfo.pushConstantRangeCount = pushRanges.size();
	createInfo.pPushConstantRanges = pushRanges.data();
	VK_CHECK(vkCreatePipelineLayout(device, &createInfo, nullptr, &pipelineLayout));
}

void Pipeline::createGraphicsPipeline(VkDevice device, VkRenderPass renderpass)
{
	gf3d::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	shaderStages.reserve(shader.getShaderModules().size());

	for (auto&& [stage, module] : shader.getShaderModules()) {
		VkPipelineShaderStageCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderInfo.module = module;
		shaderInfo.pName = "main";
		shaderInfo.stage = stage;

		shaderStages.push_back(shaderInfo);
	}

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

	gf3d::vector<VkVertexInputBindingDescription> bindings = Mesh::Vertex::getBindingDescription();
	gf3d::vector<VkVertexInputAttributeDescription> attributes = Mesh::Vertex::getAttributeDescription();
	
	VkPipelineVertexInputStateCreateInfo vertexInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
	vertexInput.pVertexBindingDescriptions = bindings.data();
	vertexInput.vertexAttributeDescriptionCount = static_cast<u32>(attributes.size());
	vertexInput.pVertexAttributeDescriptions = attributes.data();

	VkDynamicState dynamicState[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.pDynamicStates = dynamicState;

	VkPipelineDepthStencilStateCreateInfo depthStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	depthStateInfo.depthTestEnable = VK_TRUE;
	depthStateInfo.depthWriteEnable = VK_TRUE;
	depthStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStateInfo.minDepthBounds = 0.0f;
	depthStateInfo.maxDepthBounds = 1.0f;
	depthStateInfo.stencilTestEnable = VK_FALSE;

	VkGraphicsPipelineCreateInfo graphicsInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
	graphicsInfo.renderPass = renderpass;
	graphicsInfo.layout = pipelineLayout;
	graphicsInfo.stageCount = shaderStages.size();
	graphicsInfo.pStages = shaderStages.data();
	graphicsInfo.pRasterizationState = &rasterizerInfo;
	graphicsInfo.pInputAssemblyState =  &inputAssembly;
	graphicsInfo.pViewportState = &viewportStateInfo;
	graphicsInfo.pMultisampleState = &multisampleInfo;
	graphicsInfo.pColorBlendState = &blendInfo;
	graphicsInfo.pVertexInputState = &vertexInput;
	graphicsInfo.pDynamicState = &dynamicStateInfo;
	graphicsInfo.pDepthStencilState = &depthStateInfo;
	VK_CHECK(vkCreateGraphicsPipelines(device, nullptr, 1, &graphicsInfo, nullptr, &pipeline));
}
