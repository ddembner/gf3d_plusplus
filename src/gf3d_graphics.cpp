#include "gf3d_graphics.h"
#include "gf3d_validations.h"
#include "vulkan_functions.h"
#include <iostream>

using std::cout;
using std::endl;

void Gf3dGraphics::init(Gf3dWindow* window)
{
	this->window = window;
	initVulkan();
	camera.position = glm::vec3(0.f, 0.f, -2.f);
	camera.OnUpdate();
}

void Gf3dGraphics::cleanup()
{
	auto device = gf3dDevice.GetDevice();
	auto allocator = gf3dDevice.GetAllocator();

	vkDeviceWaitIdle(device);
	cleanMaterials();
	sampleMesh.destroy(allocator);
	//vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	for (uint32_t i = 0; i < MAX_FRAMES_INFLIGHT; i++) {
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderCompleteSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyCommandPool(device, transferCommandPool, nullptr);
	swapchain.cleanup(device);

	gf3dDevice.cleanup();

	cout << "Successfully cleaned graphics" << endl;
}

void Gf3dGraphics::draw()
{
	auto device = gf3dDevice.GetDevice();
	
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapchain.getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
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
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderCompleteSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	recordCommandBuffer(imageIndex);

	VK_CHECK(vkQueueSubmit(gf3dDevice.GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(gf3dDevice.GetPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
		return;
	}


	currentFrame = (currentFrame + 1) % MAX_FRAMES_INFLIGHT;
}

void Gf3dGraphics::cleanMaterials()
{
	auto device = gf3dDevice.GetDevice();
	for (auto& material : materials) {
		material.second.freeMaterial(device);
	}
}

void Gf3dGraphics::createCommandPool()
{
	auto device = gf3dDevice.GetDevice();
	VkCommandPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	createInfo.queueFamilyIndex = gf3dDevice.GetQueueIndex(VK_QUEUE_GRAPHICS_BIT);
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VK_CHECK(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool));

	VkCommandPoolCreateInfo transferCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	transferCreateInfo.queueFamilyIndex = gf3dDevice.GetQueueIndex(VK_QUEUE_TRANSFER_BIT);
	transferCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VK_CHECK(vkCreateCommandPool(device, &transferCreateInfo, nullptr, &transferCommandPool));
}

void Gf3dGraphics::createCommandBuffers()
{
	auto device = gf3dDevice.GetDevice();

	commandBuffers.resize(swapchain.imageCount());
	
	VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
}

void Gf3dGraphics::createSyncObjects()
{
	auto device = gf3dDevice.GetDevice();

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
	if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) { camera.position.z += 0.01f; }
	if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) { camera.position.z -= 0.01f; }
	camera.OnUpdate();
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VkCommandBuffer cmd = commandBuffers[imageIndex];

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
	if (sampleMesh.isAllocated) {
		sampleMesh.bind(cmd);
		sampleMesh.draw(cmd);
	}
	//vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);

	vkCmdEndRenderPass(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd));
}

void Gf3dGraphics::recreateSwapChain()
{
	VkExtent2D extent = window->getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window->getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(gf3dDevice.GetDevice());

	swapchain.recreate(gf3dDevice);
}

void Gf3dGraphics::createDescriptorPool()
{
	auto device = gf3dDevice.GetDevice();

	VkDescriptorPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool);
}

void Gf3dGraphics::createGlobalUniforms()
{

}

void Gf3dGraphics::oncePerFrameCommands(VkCommandBuffer& cmd)
{

}

void Gf3dGraphics::createMaterial(const std::string& vertPath, const std::string& fragPath)
{
	auto device = gf3dDevice.GetDevice();
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

	mesh.allocateMesh(gf3dDevice, commandPool);

	meshes["triangle"] = mesh;

	return mesh;
}

//Initializes the vulkan api
void Gf3dGraphics::initVulkan() 
{
	gf3dDevice.init(window);

	swapchain.init(gf3dDevice);
	
	createCommandPool();

	createCommandBuffers();

	createSyncObjects();

	//createDescriptorPool();

	createMaterial("D:/Programming/gf3d++/gf3d++/shaders/vert.spv", "D:/Programming/gf3d++/gf3d++/shaders/frag.spv");
}