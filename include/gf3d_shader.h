#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

class Shader
{
public:
	Shader(VkDevice _device, const std::string& filepath);
	Shader(const std::string& vertPath, const std::string& fragPath);
	void destroy();
	std::unordered_map<VkShaderStageFlagBits, VkShaderModule> getShaderModules() const { return shaderModules; }
private:
	bool checkIfAlreadyCompiled(const std::string& filepath);
	std::string readFile(const std::string& filepath);
	std::unordered_map<VkShaderStageFlagBits, std::string> getShaderSources(const std::string& source);
	void compileShadersToSpv();
	void loadShaderModule(VkShaderStageFlagBits stage, const std::vector<uint32_t>& codeData);
private:
	VkDevice device;
	bool isVertCompiled = false;
	bool isFragCompiled = false;
	std::string pathToFile;
	std::unordered_map<VkShaderStageFlagBits, std::string> shaderSources;
	std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shaderModules;
};