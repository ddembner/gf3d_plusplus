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
	Gf3dGraphics() {}
	~Gf3dGraphics() {}
	void init(Gf3dWindow* window);
	void cleanup();
	void draw();
private:
	Swapchain swapchain;
	VkCommandPool commandPool;
	VkCommandPool transferCommandPool;
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
	size_t currentFrame = 0;

	Mesh sampleMesh;
	Camera camera;
	Gf3dWindow* window;
	Gf3dDevice gf3dDevice;

public:
	void createMaterial(const std::string& vertPath, const std::string& fragPath);
	void loadModel();
	Mesh createMesh(const std::string& path);
private:
	void initVulkan();
	void cleanMaterials();

	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void recordCommandBuffer(uint32_t imageIndex);
	void recreateSwapChain();
	void createDescriptorPool();
	void createGlobalUniforms();
	void oncePerFrameCommands(VkCommandBuffer& cmd);
};
