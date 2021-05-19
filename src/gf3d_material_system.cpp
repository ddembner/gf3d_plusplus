#include "gf3d_logger.h"
#include "gf3d_material_system.h"

template<> MaterialSystem*  Singleton<MaterialSystem>::instance = nullptr;

MaterialSystem::MaterialSystem()
{
}

MaterialSystem::~MaterialSystem()
{
}

void MaterialSystem::init(Gf3dDevice* device, VkRenderPass renderpass)
{
	gf3dDevice = device;
	this->renderpass = renderpass;
}

void MaterialSystem::destroy()
{
	for (auto& material : materials) {
		material.second->freeMaterial(gf3dDevice->GetDevice());
	}
}

Material* MaterialSystem::create(const std::string& shaderPath)
{
	Material* newMat = find(shaderPath);

	if (newMat) {
		return newMat;
	}

	newMat = new Material(gf3dDevice->GetDevice(), renderpass, shaderPath);
	materials[shaderPath] = newMat;

	return newMat;
}

Material* MaterialSystem::create(const std::string& vertPath, const std::string& fragPath)
{
	std::string name = vertPath + fragPath;

	Material* newMat = find(name);

	if (newMat) {
		return newMat;
	}

	//newMat = new Material();
	//newMat->pipeline.loadPipeline(*gf3dDevice, renderpass, vertPath, fragPath);
	//materials[name] = newMat;

	//return newMat;

	return nullptr;
}

Material* MaterialSystem::find(const std::string& name)
{
	if (materials.find(name) != materials.end()) {
		return	materials[name];
	}
	return nullptr;
}
