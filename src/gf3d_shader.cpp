#include <shaderc/shaderc.hpp>
#include "gf3d_logger.h"
#include "gf3d_shader.h"
#include "vulkan_functions.h"

#include <fstream>

Shader::Shader(VkDevice _device, const std::string& filepath) : device(_device)
{
    //This should return true when a file tracker is implemented for shaders
    if (checkIfAlreadyCompiled(filepath)) {
        LOGGER_DEBUG("ALREADY COMPILED");
    }
    else { 
        pathToFile = filepath;
        std::string shaderFile = readFile(filepath);
        shaderSources = getShaderSources(shaderFile);
        if (shaderSources.size() == 0) {
            LOGGER_WARN("No shader stages found in shader file: {}", filepath);
        }
        compileShadersToSpv();
    }
}

void Shader::destroy()
{
    for (auto&& [stage, module] : shaderModules) {
        vkDestroyShaderModule(device, module, nullptr);
    }
}

static VkShaderStageFlagBits stringToVulkanShaderFlag(const std::string& stringType)
{
    if(stringType == "vertex") return VK_SHADER_STAGE_VERTEX_BIT;
    if(stringType == "fragment") return VK_SHADER_STAGE_FRAGMENT_BIT;

    LOGGER_ERROR("Unknown shader type: {}", stringType);
    return (VkShaderStageFlagBits)0;
}

static const char* shaderStageToString(VkShaderStageFlagBits stage) 
{
    switch(stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT: return "vertex";
        case VK_SHADER_STAGE_FRAGMENT_BIT: return "fragment";
        default: return nullptr;
    }
}

static const char* shaderStageToFileExtention(VkShaderStageFlagBits stage)
{
    switch(stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT: return ".vert";
        case VK_SHADER_STAGE_FRAGMENT_BIT: return ".frag";
        default: return nullptr;
    }
}

static shaderc_shader_kind shaderStageToShadercKind(VkShaderStageFlagBits stage)
{
    switch(stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT: return shaderc_vertex_shader;
        case VK_SHADER_STAGE_FRAGMENT_BIT: return shaderc_fragment_shader;
        default: return (shaderc_shader_kind)-1;
    }
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

std::unordered_map<VkShaderStageFlagBits, std::string> Shader::getShaderSources(const std::string& source)
{
    std::unordered_map<VkShaderStageFlagBits, std::string> sources;
    const char* token = "#type ";
    size_t tokenLen = strlen(token);
    size_t pos = source.find(token, 0);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + tokenLen;
        std::string stringType = source.substr(begin, eol - begin);
        VkShaderStageFlagBits shaderStage = stringToVulkanShaderFlag(stringType);
        if (!shaderStage) {
            LOGGER_ERROR("Failed to find supported shader stage in shader");
            return std::unordered_map<VkShaderStageFlagBits, std::string>();
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
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);
    options.SetTargetSpirv(shaderc_spirv_version_1_3);
    for (auto&& [stage, source] : shaderSources) {
        std::string finalPath = pathToFile + shaderStageToFileExtention(stage);
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderStageToShadercKind(stage), pathToFile.c_str(), options);

        if(result.GetCompilationStatus() != shaderc_compilation_status_success) {
            LOGGER_ERROR(result.GetErrorMessage());
        }

        loadShaderModule(stage, std::vector<uint32_t>(result.cbegin(), result.cend()));

        // Uncomment then when the shader code should be cahced for future use instead of being recompiled
        // std::ofstream out(finalPath, std::ios::out | std::ios::binary);
        // if (out.is_open()) {
        //     std::vector<uint32_t> shaderData = std::vector<uint32_t>(result.cbegin(), result.cend());
        //     out.write((char*)shaderData.data(), shaderData.size() * sizeof(uint32_t));
        //     out.flush();
        //     out.close();
        // }

    }
}

void Shader::loadShaderModule(VkShaderStageFlagBits stage, const std::vector<uint32_t>& codeData)
{
    VkShaderModuleCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    shaderInfo.codeSize = codeData.size() * sizeof(uint32_t);
    shaderInfo.pCode = codeData.data();

    VkShaderModule newModule;

    VK_CHECK( vkCreateShaderModule(device, &shaderInfo, nullptr, &newModule));

    shaderModules[stage] = newModule;
}
