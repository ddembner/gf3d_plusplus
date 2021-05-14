#pragma once
#include "gf3d_singleton.h"
#include "gf3d_material.h"
#include <unordered_map>

class MaterialSystem : public Singleton<MaterialSystem>
{
public:
	MaterialSystem();
	~MaterialSystem();
	void init(Gf3dDevice* device, VkRenderPass renderpass);
	void destroy();
	Material* create(const std::string& vertPath, const std::string& fragPath);
	Material* find(const std::string& name);
private:
	Gf3dDevice* gf3dDevice = nullptr;
	VkRenderPass renderpass = VK_NULL_HANDLE;
	std::unordered_map<std::string, Material*> materials;
};