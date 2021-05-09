#pragma once

#include <cassert>
#include <unordered_map>
#include "gf3d_device.h"
#include "gf3d_swapchain.h"
#include "gf3d_pipeline.h"
#include "gf3d_mesh.h"
#include "gf3d_camera.h"

class Gf3dGraphics
{
public:
	Gf3dGraphics() = default;
	~Gf3dGraphics() = default;
	void init(Gf3dWindow* const window, Gf3dDevice* device);
	void cleanup();
	void draw();
private:
	Swapchain swapchain;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderCompleteSemaphores;
	std::vector<VkFence> imagesInFlight;
	std::vector<VkFence> inFlightFences;
	std::unordered_map<std::string, Material> materials;
	std::unordered_map<std::string, Mesh> meshes;
	struct {
		uint32_t graphics;
		uint32_t compute;
		uint32_t transfer;
	} queueIndices;
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout globalLayout;

	const uint32_t MAX_FRAMES_INFLIGHT = 2;
	uint32_t currentImageIndex;
	int currentFrame = 0;

	Mesh sampleMesh;
	Camera camera;
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
	void createMaterial(const std::string& vertPath, const std::string& fragPath);
	void loadModel();
	Mesh createMesh(const std::string& path);
	VkCommandBuffer beginFrame();
private:
	void initVulkan();
	void cleanMaterials();
	void createCommandBuffers();
	void createSyncObjects();
	void recordCommandBuffer(uint32_t imageIndex);
	void recreateSwapChain();
	void createDescriptorPool();
	void createGlobalUniforms();
	void createPerFrameData();
	void oncePerFrameCommands(VkCommandBuffer& cmd);
	VkCommandBuffer getCurrentCommandBuffer() { return commandBuffers[currentFrame]; }
};
