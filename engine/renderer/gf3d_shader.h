#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <map>
#include <vector>

struct Uniform
{
	uint32_t offset = 0;
	uint32_t size = 0;
	uint32_t type = 0;
	uint32_t vecsize = 1;
	uint32_t column = 1;
};

class Shader
{
public:
	Shader(VkDevice _device, const std::string& pathToFile);
	void destroy();
	std::map<VkShaderStageFlagBits, VkShaderModule> getShaderModules() const { return shaderModules; }
	std::vector<VkPushConstantRange> getPushConstantRanges() const { return pushConstantRanges; }
	uint32_t getPushDataSize();

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
	std::vector<uint32_t> compileSourceToSpirv(VkShaderStageFlagBits stage, const std::string& source);
	void loadShaderModule(VkShaderStageFlagBits stage, const std::vector<uint32_t>& codeData);
	std::string getShaderFileFinalNameForStage(VkShaderStageFlagBits stage);
	void Reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& data);
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