#pragma once
#include "gf3d_device.h"
#include "vulkan_types.h"
#include "containers/vector.hpp"
#include <unordered_map>

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

		static gf3d::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		static gf3d::vector<VkVertexInputBindingDescription> getBindingDescription();
	};
	AllocatedBuffer allocatedBuffer;
	gf3d::vector<Vertex> vertices;
	bool isAllocated = false;
public:
	void destroy(const VmaAllocator& allocator);
	void allocateMesh(Gf3dDevice& gf3dDevice);
	void bind(VkCommandBuffer cmd);
	void draw(VkCommandBuffer cmd);
	u32 getVertexCount() const { return vertexCount; }
private:
	u32 vertexCount = 0;
};

class MeshFactory
{
public:
	static Mesh* Create(std::string& filepath) {
		return nullptr;
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
		}
		return false;
	}

private:
	static std::unordered_map<std::string, Mesh*> meshes;
};