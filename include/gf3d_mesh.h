#pragma once
#include "gf3d_device.h"
#include <vector>
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "vulkan_types.h"

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	Mesh(const std::string& path);
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
	};
	AllocatedBuffer allocatedBuffer;
	bool isAllocated = false;
public:
	void destroy(const VmaAllocator& allocator);
	void allocateMesh(Gf3dDevice& gf3dDevice, VkCommandPool commandPool);
	void bind(VkCommandBuffer cmd);
	void draw(VkCommandBuffer cmd);
	uint32_t getVertexCount() const { return vertexCount; }
private:
	std::vector<Vertex> vertices;
	uint32_t vertexCount;
};

class MeshFactory
{
public:
	static Mesh* Create(std::string& filepath) {

	}

	static bool Destroy(std::string& filepath) {
		auto it = meshes.find(filepath);
		if (it != meshes.end()) {
			delete it->second;
			return true;
		}
		return false;
	}

	static bool Destroy(Mesh* mesh) {
		for (auto it = meshes.begin(); it != meshes.end(); ++it) {
			if (it != meshes.end()) {
				delete it->second;
				return true;
			}
			return false;
		}
	}

private:
	static std::unordered_map<std::string, Mesh*> meshes;
};