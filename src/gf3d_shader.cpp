#include "gf3d_logger.h"
#include "gf3d_shader.h"

#include <fstream>
#include <filesystem>
#include <vector>

#include <shaderc/shaderc.hpp>

Shader::Shader(const std::string& filepath)
{
    if (checkIfAlreadyCompiled(filepath)) {
        LOGGER_DEBUG("ALREADY COMPILED");
    }
    else { 
        std::string shaderFile = readFile(filepath);
        shaderSources = getShaderSources(shaderFile);
        if (shaderSources.size() == 0) {
            LOGGER_WARN("No shader stages found in shader file: {}", filepath);
        }
        compileShadersToSpv();
    }
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{

}

static VkShaderStageFlags stringToVulkanShaderFlag(const std::string& stringType)
{
    if(stringType == "vertex") return VK_SHADER_STAGE_VERTEX_BIT;
    if(stringType == "fragment") return VK_SHADER_STAGE_FRAGMENT_BIT;

    LOGGER_ERROR("Unknown shader type: {}", stringType);
    return 0;
}

bool Shader::checkIfAlreadyCompiled(const std::string& filepath)
{
    std::ifstream vertFile(filepath + ".vert", std::ios::in);
    std::ifstream fragFile(filepath + ".frag", std::ios::in);
    
    isVertCompiled = vertFile.is_open();
    isFragCompiled = fragFile.is_open();

    if (isVertCompiled && isFragCompiled) {
        return true;
    }

    return false;
}

std::string Shader::readFile(const std::string& filepath)
{
    std::string fileContent;
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        int size = file.tellg();

        if (size != -1) {
            fileContent.resize(size);
            file.seekg(0, std::ios::beg);
            file.read(&fileContent[0], size);
        }
        else {
            LOGGER_ERROR("Failed to read file: {}", filepath);
        }
    }
    else { 
        LOGGER_ERROR("Failed to load shader file: {}", filepath);
    }
    return fileContent;
}

std::unordered_map<VkShaderStageFlags, std::string> Shader::getShaderSources(const std::string& source)
{
    std::unordered_map<VkShaderStageFlags, std::string> sources;
    const char* token = "#type ";
    size_t tokenLen = strlen(token);
    size_t pos = source.find(token, 0);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + tokenLen;
        std::string stringType = source.substr(begin, eol - begin);
        VkShaderStageFlags shaderStage = stringToVulkanShaderFlag(stringType);
        if (!shaderStage) {
            LOGGER_ERROR("Failed to find supported shader stage in shader");
            return std::unordered_map<VkShaderStageFlags, std::string>();
        }
        size_t nextLine = source.find_first_not_of("\r\n", eol);
        size_t nextPos = source.find(token, eol);
        pos = nextPos;
        sources[shaderStage] = source.substr(nextLine, nextPos - nextLine);
    }

    return sources;
}

void Shader::compileShadersToSpv()
{
    //shaderc::Compiler compiler;
}
