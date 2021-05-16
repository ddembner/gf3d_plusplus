#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertPath, const std::string& fragPath);
private:
	bool checkIfAlreadyCompiled(const std::string& filepath);
	std::string readFile(const std::string& filepath);
	std::unordered_map<VkShaderStageFlags, std::string> getShaderSources(const std::string& source);
	void compileShadersToSpv();
private:
	bool isVertCompiled = false;
	bool isFragCompiled = false;
	VkShaderModule vertModule = 0;
	VkShaderModule fragModule = 0;
	std::unordered_map<VkShaderStageFlags, std::string> shaderSources;
};