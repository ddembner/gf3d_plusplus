#include "gf3d_graphics.h"
#include "gf3d_validations.h"
#include "vulkan_functions.h"
#include <iostream>

using std::cout;
using std::endl;

void Gf3dGraphics::init(Gf3dWindow* window, Gf3dDevice* device)
{
	gf3dWindow = window;
	gf3dDevice = device;
	initVulkan();
	camera.position = glm::vec3(0.f, 0.f, -2.f);
	camera.OnUpdate();
}

void Gf3dGraphics::cleanup()
{
	auto device = gf3dDevice->GetDevice();
	auto allocator = gf3dDevice->GetAllocator();

	vkDeviceWaitIdle(device);
	cleanMaterials();
	sampleMesh.destroy(allocator);

	for (int i = 0; i < frameData.size(); i++) {
		vmaDestroyBuffer(gf3dDevice->GetAllocator(), frameData[i].cameraBuffer.buffer, frameData[i].cameraBuffer.allocation);
		vkFreeDescriptorSets(device, descriptorPool, 1, &frameData[i].globalSet);
	}

	vkDestroyDescriptorSetLayout(device, globalLayout, nullptr);
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	for (uint32_t i = 0; i < MAX_FRAMES_INFLIGHT; i++) {
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderCompleteSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkFreeCommandBuffers(device, gf3dDevice->GetCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	swapchain.cleanup(device);

	cout << "Successfully cleaned graphics" << endl;
}

void Gf3dGraphics::draw()
{
	auto device = gf3dDevice->GetDevice();
	
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapchain.getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
	}
	
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { renderCompleteSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	recordCommandBuffer(imageIndex);

	VK_CHECK(vkQueueSubmit(gf3dDevice->GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(gf3dDevice->GetGraphicsQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	}


	currentFrame = (currentFrame + 1) % MAX_FRAMES_INFLIGHT;
}

void Gf3dGraphics::cleanMaterials()
{
	auto device = gf3dDevice->GetDevice();
	for (auto& material : materials) {
		material.second.freeMaterial(device);
	}
}

void Gf3dGraphics::createCommandBuffers()
{
	auto device = gf3dDevice->GetDevice();

	commandBuffers.resize(MAX_FRAMES_INFLIGHT);
	
	VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandPool = gf3dDevice->GetCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VK_CHECK(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()));
}

void Gf3dGraphics::createSyncObjects()
{
	auto device = gf3dDevice->GetDevice();

	imageAvailableSemaphores.resize(MAX_FRAMES_INFLIGHT);
	renderCompleteSemaphores.resize(MAX_FRAMES_INFLIGHT);
	inFlightFences.resize(MAX_FRAMES_INFLIGHT);
	imagesInFlight.resize(swapchain.imageCount());

	VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

	VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < MAX_FRAMES_INFLIGHT; i++) {
		VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
		VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderCompleteSemaphores[i]));
		VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]));
	}
}

void Gf3dGraphics::recordCommandBuffer(uint32_t imageIndex)
{
	if (glfwGetKey(gf3dWindow->getWindow(), GLFW_KEY_W) == GLFW_PRESS) { camera.position.z += 0.01f; }
	if (glfwGetKey(gf3dWindow->getWindow(), GLFW_KEY_S) == GLFW_PRESS) { camera.position.z -= 0.01f; }
	camera.OnUpdate();
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VkCommandBuffer cmd = commandBuffers[currentFrame];

	VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

	oncePerFrameCommands(cmd);

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapchain.getRenderPass();
	renderPassInfo.framebuffer = swapchain.getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapchain.getExtent();

	VkClearValue clearColor = { 0.2f, 0.2f, 0.3f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	
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
	vkCmdSetViewport(cmd, 0, 1, &viewport);
	vkCmdSetScissor(cmd, 0, 1, &scissor);
	

	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, materials.begin()->second.getGraphicsPipeline());
	//vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, materials.begin()->second.getPipelineLayout(), 0, 0, nullptr, 0, nullptr);
	if (sampleMesh.isAllocated) {
		sampleMesh.bind(cmd);
		sampleMesh.draw(cmd);
	}

	vkCmdEndRenderPass(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd));
}

void Gf3dGraphics::recreateSwapChain()
{
	VkExtent2D extent = gf3dWindow->getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = gf3dWindow->getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(gf3dDevice->GetDevice());

	swapchain.recreate(*gf3dDevice);
}

void Gf3dGraphics::createDescriptorPool()
{
	auto device = gf3dDevice->GetDevice();
	std::vector<VkDescriptorPoolSize> poolSizes = { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10} };

	VkDescriptorPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	createInfo.pPoolSizes = poolSizes.data();
	createInfo.maxSets = 10;
	createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	VK_CHECK(vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool));

	VkDescriptorSetLayoutBinding cameraBinding = {};
	cameraBinding.binding = 0;
	cameraBinding.descriptorCount = 1;
	cameraBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	cameraBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &cameraBinding;
	
	VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &globalLayout));

	for (int i = 0; i < frameData.size(); i++) {

		VkDescriptorSetAllocateInfo globalAllocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		globalAllocInfo.descriptorPool = descriptorPool;
		globalAllocInfo.descriptorSetCount = 1;
		globalAllocInfo.pSetLayouts = &globalLayout;
		VK_CHECK(vkAllocateDescriptorSets(device, &globalAllocInfo, &frameData[i].globalSet));

		VkDescriptorBufferInfo cameraBufferInfo = {};
		cameraBufferInfo.buffer = frameData[i].cameraBuffer.buffer;
		cameraBufferInfo.offset = 0;
		cameraBufferInfo.range = sizeof(GPUCameraData);

		VkWriteDescriptorSet cameraWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		cameraWrite.descriptorCount = 1;
		cameraWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		cameraWrite.dstSet = frameData[i].globalSet;
		cameraWrite.dstBinding = 0;
		cameraWrite.pBufferInfo = &cameraBufferInfo;

		vkUpdateDescriptorSets(device, 1, &cameraWrite, 0, nullptr);
	}
}

void Gf3dGraphics::createGlobalUniforms()
{

}

void Gf3dGraphics::createPerFrameData()
{
	frameData.resize(MAX_FRAMES_INFLIGHT);

	for (int i = 0; i < frameData.size(); i++) {
		frameData[i].cameraBuffer = createBuffer(gf3dDevice->GetAllocator(), sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
	}
}

void Gf3dGraphics::oncePerFrameCommands(VkCommandBuffer& cmd)
{
	auto allocator = gf3dDevice->GetAllocator();

	void* data;
	vmaMapMemory(allocator, frameData[currentFrame].cameraBuffer.allocation, &data);

	memcpy(data, &camera, sizeof(GPUCameraData));

	vmaUnmapMemory(allocator, frameData[currentFrame].cameraBuffer.allocation);
}

void Gf3dGraphics::createMaterial(const std::string& vertPath, const std::string& fragPath)
{
	auto device = gf3dDevice->GetDevice();
	//TODO: Make better system in which materials are named/made
	std::string uniqueMaterialName = vertPath + fragPath;
	if (materials.find(uniqueMaterialName) != materials.end()) {
		cout << "Material already exists" << std::endl;
		return;
	}
	Material material(swapchain, device, vertPath, fragPath);
	materials[uniqueMaterialName] = material;
}

void Gf3dGraphics::loadModel()
{
	if (sampleMesh.isAllocated) return;

	sampleMesh = createMesh("");
}

Mesh Gf3dGraphics::createMesh(const std::string& path)
{
	Mesh mesh;

	mesh.allocateMesh(*gf3dDevice, gf3dDevice->GetCommandPool());

	meshes["triangle"] = mesh;

	return mesh;
}

//Initializes the vulkan api
void Gf3dGraphics::initVulkan() 
{
	swapchain.init(*gf3dDevice);

	createCommandBuffers();

	createSyncObjects();

	createPerFrameData();

	createDescriptorPool();

	createMaterial("D:/Programming/gf3d++/gf3d++/shaders/vert.spv", "D:/Programming/gf3d++/gf3d++/shaders/frag.spv");
}