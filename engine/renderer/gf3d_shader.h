#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <map>
#include <vector>
#include "defines.hpp"

struct Uniform
{
	u32 offset = 0;
	u32 size = 0;
	u32 type = 0;
	u32 vecsize = 1;
	u32 column = 1;
};

class Shader
{
public:
	Shader(VkDevice _device, const std::string& pathToFile);
	void destroy();
	std::map<VkShaderStageFlagBits, VkShaderModule> getShaderModules() const { return shaderModules; }
	std::vector<VkPushConstantRange> getPushConstantRanges() const { return pushConstantRanges; }
	u32 getPushDataSize();

	Uniform getUniform(const std::string& name) const 
	{
		auto it = uniforms.find(name);

		if (it == uniforms.end()) {
			return Uniform();
		}

		return it->second;
	}

private:
	bool checkIfAlreadyCompiled();
	std::string readFile();
	std::map<VkShaderStageFlagBits, std::string> getShaderSources(const std::string& source);
	void readPreCompiledFiles();
	void compileShadersToSpv();
	std::vector<u32> compileSourceToSpirv(VkShaderStageFlagBits stage, const std::string& source);
	void loadShaderModule(VkShaderStageFlagBits stage, const std::vector<u32>& codeData);
	std::string getShaderFileFinalNameForStage(VkShaderStageFlagBits stage);
	void Reflect(VkShaderStageFlagBits stage, std::vector<u32>& data);
private:
	VkDevice device;
	std::string filepath;
	std::map<VkShaderStageFlagBits, std::string> shaderSources;
	std::map<VkShaderStageFlagBits, VkShaderModule> shaderModules;
	std::vector<VkPushConstantRange> pushConstantRanges;
	std::map<VkShaderStageFlagBits, std::vector<Uniform>> pushMemberData;
	std::map<std::string, Uniform> uniforms;
};

inline bool operator==(const Uniform& lhs, const Uniform& rhs) 
{
	return (
		lhs.size == rhs.size && 
		lhs.offset == rhs.offset && 
		lhs.type == rhs.type && 
		lhs.vecsize == rhs.vecsize &&
		lhs.column == rhs.column
	);
}

inline bool operator!=(const Uniform& lhs, const Uniform& rhs) 
{ 
	return !(lhs == rhs); 
}