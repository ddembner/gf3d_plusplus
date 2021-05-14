#pragma once
#include <vulkan/vulkan.h>
#include <string>

class Shader
{
public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertPath, const std::string& fragPath);
private:
	VkShaderModule vertModule = 0;
	VkShaderModule fragModule = 0;
};