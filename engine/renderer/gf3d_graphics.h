#pragma once

#include "core/gf3d_camera.h"
#include "core/GameObject.h"

class Gf3dGraphics
{
public:
	Gf3dGraphics() = default;
	~Gf3dGraphics() = default;
	void init(Gf3dWindow* const window, Gf3dDevice* device);
	void cleanup();
	VkRenderPass getSwapchainRenderPass() const { return swapchain.getRenderPass(); }
private:
	Swapchain swapchain;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderCompleteSemaphores;
	std::vector<VkFence> imagesInFlight;
	std::vector<VkFence> inFlightFences;
	std::unordered_map<std::string, Mesh> meshes;
	struct {
		u32 graphics;
		u32 compute;
		u32 transfer;
	} queueIndices;
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout globalLayout;

	const u32 MAX_FRAMES_INFLIGHT = 2;
	u32 currentImageIndex;
	int currentFrame = 0;
	bool isFrameInProgress = false;

	Gf3dWindow* gf3dWindow;
	Gf3dDevice* gf3dDevice;

	struct GPUCameraData
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewProjectionMatrix;
	};

	struct PerFrameData
	{
		AllocatedBuffer cameraBuffer;
		VkDescriptorSet globalSet;
	};

	std::vector<PerFrameData> frameData;

public:
	VkCommandBuffer beginFrame();
	void endFrame();
	void beginRenderPass(VkCommandBuffer cmd);
	void endRenderPass(VkCommandBuffer cmd);
	void renderObjects(VkCommandBuffer cmd, std::vector<GameObject>& gameObjects);
	void updateCameraDescriptor(Camera& camera);
private:
	void initVulkan();
	void createCommandBuffers();
	void createSyncObjects();
	void recreateSwapChain();
	void createDescriptorPool();
	void createGlobalUniforms();
	void createPerFrameData();
	void oncePerFrameCommands(VkCommandBuffer& cmd);
	VkCommandBuffer getCurrentCommandBuffer() { return commandBuffers[currentFrame]; }
};
