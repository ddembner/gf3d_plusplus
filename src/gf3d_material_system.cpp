#include "gf3d_logger.h"
#include "gf3d_material_system.h"

MaterialSystem* MaterialSystem::instance = nullptr;

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
		material.second->pipeline.destroyPipeline(gf3dDevice->GetDevice());
	}
}

Material* MaterialSystem::create(const std::string& vertPath, const std::string& fragPath)
{
	std::string name = vertPath + fragPath;

	Material* newMat = find(name);

	if (newMat) {
		LOGGER_DEBUG("Material already exists");
		return newMat;
	}

	newMat = new Material();
	newMat->pipeline.loadPipeline(*gf3dDevice, renderpass, vertPath, fragPath);
	materials[name] = newMat;

	return newMat;
}

Material* MaterialSystem::find(const std::string& name)
{
	if (materials.find(name) != materials.end()) {
		return	materials[name];
	}
	return nullptr;
}
