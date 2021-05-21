#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

struct ShaderInfo
{

};

class Shader
{
public:
	Shader(VkDevice _device, const std::string& pathToFile);
	Shader(const std::string& vertPath, const std::string& fragPath);
	void destroy();
	std::unordered_map<VkShaderStageFlagBits, VkShaderModule> getShaderModules() const { return shaderModules; }
	std::vector<VkPushConstantRange> getPushConstantRanges() const { return pushConstantRanges; }
private:
	bool checkIfAlreadyCompiled();
	std::string readFile();
	std::unordered_map<VkShaderStageFlagBits, std::string> getShaderSources(const std::string& source);
	void readPreCompiledFiles();
	void compileShadersToSpv();
	void loadShaderModule(VkShaderStageFlagBits stage, const std::vector<uint32_t>& codeData);
	std::string getShaderFileFinalNameForStage(VkShaderStageFlagBits stage);
	void Reflect(VkShaderStageFlagBits stage, const std::vector<uint32_t>& data);
private:
	VkDevice device;
	std::string filepath;
	std::unordered_map<VkShaderStageFlagBits, std::string> shaderSources;
	std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shaderModules;
	std::unordered_map<VkShaderStageFlagBits, ShaderInfo> shaderInfos;
	std::vector<VkPushConstantRange> pushConstantRanges;
};