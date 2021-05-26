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

VkCommandBuffer Gf3dGraphics::beginFrame()
{
	auto device = gf3dDevice->GetDevice();

	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(device, swapchain.getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return VK_NULL_HANDLE;
	}

	isFrameInProgress = true;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VkCommandBuffer cmd = getCurrentCommandBuffer();

	VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

	return cmd;
}

void Gf3dGraphics::endFrame()
{
	auto device = gf3dDevice->GetDevice();
	auto cmd = getCurrentCommandBuffer();

	VK_CHECK(vkEndCommandBuffer(cmd));

	if (imagesInFlight[currentImageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[currentImageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[currentImageIndex] = inFlightFences[currentFrame];

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

	VK_CHECK(vkQueueSubmit(gf3dDevice->GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &currentImageIndex;

	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(gf3dDevice->GetGraphicsQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	}

	isFrameInProgress = false;
	currentFrame = (currentFrame + 1) % MAX_FRAMES_INFLIGHT;
}

void Gf3dGraphics::beginRenderPass(VkCommandBuffer cmd)
{
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapchain.getRenderPass();
	renderPassInfo.framebuffer = swapchain.getFrameBuffer(currentImageIndex);
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
}

void Gf3dGraphics::endRenderPass(VkCommandBuffer cmd)
{
	vkCmdEndRenderPass(cmd);
}

void Gf3dGraphics::renderObjects(VkCommandBuffer cmd, std::vector<GameObject>& gameObjects)
{
	Material* lastMaterial = nullptr;
	for (auto& gameObject : gameObjects) {
		if (lastMaterial != gameObject.material) {
			gameObject.material->bindPipeline(cmd);
		}

		gameObject.material->pushUpdate("mvp", &gameObject.transform);
		gameObject.material->pushUpdate("color", &gameObject.color);

		gameObject.material->submitPushConstant(cmd);
		gameObject.mesh.bind(cmd);
		gameObject.mesh.draw(cmd);
	}
}

//Initializes the vulkan api
void Gf3dGraphics::initVulkan() 
{
	swapchain.init(*gf3dDevice);

	createCommandBuffers();

	createSyncObjects();

	createPerFrameData();

	createDescriptorPool();

	std::string vertPath = ASSETS_PATH "shaders/vert.spv";
	std::string fragPath = ASSETS_PATH "shaders/frag.spv";
}