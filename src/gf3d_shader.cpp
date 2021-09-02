#include <glslang/SPIRV/GlslangToSpv.h>
#include <spirv_glsl.hpp>
#include "gf3d_logger.h"
#include "gf3d_shader.h"
#include "vulkan_functions.h"

#include <fstream>
#include <filesystem>

Shader::Shader(VkDevice _device, const std::string& pathToFile) : device(_device), filepath(pathToFile)
{
    std::string shaderFile = readFile();
    shaderSources = getShaderSources(shaderFile);
    if (shaderSources.size() == 0) {
        LOGGER_WARN("No shader stages found in shader file: {}", filepath);
    }

    if (checkIfAlreadyCompiled()) {
        readPreCompiledFiles();
    }
    else {    
        compileShadersToSpv();
    }

    shaderSources.clear();
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

static void getUniformType(const spirv_cross::SPIRType& type, Uniform& outMemberData)
{
    outMemberData.type = type.basetype;
    outMemberData.column = type.columns;
    outMemberData.vecsize = type.vecsize;
}

static EShLanguage shaderStageToGlslangKind(VkShaderStageFlagBits stage)
{
    switch(stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT: return EShLangFragment;
        case VK_SHADER_STAGE_FRAGMENT_BIT: return EShLangFragment;
        default: return (EShLanguage)-1;
    }
}

uint32_t Shader::getPushDataSize()
{
    uint32_t total = 0;
    for (const auto& pushConstantRange : pushConstantRanges) {
        total += pushConstantRange.size;
    }
    return total;
}

bool Shader::checkIfAlreadyCompiled()
{
    for (auto&& [stage, source] : shaderSources) {
        std::string shaderCompiledPath = getShaderFileFinalNameForStage(stage);
        std::ifstream in(shaderCompiledPath, std::ios::in);

        // File doesn't exist
        if (!in.is_open()) return false;

        // File is newer than the compiled versions and needs to be recompiled
        auto shaderFileTime = std::filesystem::last_write_time(filepath);
        auto shaderCompiledFileTime = std::filesystem::last_write_time(shaderCompiledPath);
        if (shaderFileTime > shaderCompiledFileTime) return false;
        
    }

    return true;
}

std::string Shader::readFile()
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

std::map<VkShaderStageFlagBits, std::string> Shader::getShaderSources(const std::string& source)
{
    std::map<VkShaderStageFlagBits, std::string> sources;
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
            return std::map<VkShaderStageFlagBits, std::string>();
        }
        size_t nextLine = source.find_first_not_of("\r\n", eol);
        size_t nextPos = source.find(token, eol);
        pos = nextPos;
        sources[shaderStage] = source.substr(nextLine, nextPos - nextLine);
    }

    return sources;
}

void Shader::readPreCompiledFiles()
{
    for (auto&& [stage, source] : shaderSources) {
        std::ifstream in(getShaderFileFinalNameForStage(stage), std::ios::in | std::ios::binary);

        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        in.seekg(0, std::ios::beg);

        std::vector<uint32_t> data(size / sizeof(uint32_t));
        in.read((char*)data.data(), size);
        
        Reflect(stage, data);

        loadShaderModule(stage, data);
    }
}

void Shader::compileShadersToSpv()
{
    for (auto&& [stage, source] : shaderSources) {
        
        std::vector shaderData = compileSourceToSpirv(stage, source);
         
        Reflect(stage, shaderData);

        loadShaderModule(stage, shaderData);
    }
}

std::vector<uint32_t> Shader::compileSourceToSpirv(VkShaderStageFlagBits stage, const std::string& source)
{
    glslang::InitializeProcess();
    
    std::vector<uint32_t> shaderData;

    std::ofstream out(getShaderFileFinalNameForStage(stage), std::ios::out | std::ios::binary);
    if (out.is_open()) {
        out.write((char*)shaderData.data(), shaderData.size() * sizeof(uint32_t));
        out.flush();
        out.close();
    }

    glslang::FinalizeProcess();

    return shaderData;
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

std::string Shader::getShaderFileFinalNameForStage(VkShaderStageFlagBits stage)
{
    std::string path = filepath.substr(0, filepath.find_last_of("/\\") + 1);
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    filename = filename.substr(0, filename.find_first_of('.'));
    return path + filename + shaderStageToFileExtention(stage) + ".spv";
}

void Shader::Reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& data)
{
    std::unique_ptr<spirv_cross::CompilerGLSL> compiler = std::make_unique<spirv_cross::CompilerGLSL>(data);

    spirv_cross::CompilerGLSL::Options options;
    options.vulkan_semantics = true;
    compiler->set_common_options(options);
    spirv_cross::ShaderResources resources = compiler->get_shader_resources();

    // Push Constants
    for (auto& resource : resources.push_constant_buffers) {
        
        const auto& bufferType = compiler->get_type(resource.base_type_id);
        uint32_t bufferSize = compiler->get_declared_struct_size(bufferType);
        std::vector<Uniform> pushData(bufferType.member_types.size());

        if (pushData.size() == 0) {
            LOGGER_WARN("Push constant struct has 0 members");
            return;
        }

        //Get uniform member data
        for (int i = 0; i < bufferType.member_types.size(); i++) {
            pushData[i].offset = compiler->type_struct_member_offset(bufferType, i);
            pushData[i].size = compiler->get_declared_struct_member_size(bufferType, i);
            getUniformType(compiler->get_type(bufferType.member_types[0]), pushData[i]);
        }
        
        //check to see if the push range would be identical
        for (auto&& [shaderStage, pushMembers] : pushMemberData) {
            for (int i = 0; i < pushMembers.size(); i++) {
                if (pushMembers[i] != pushData[i])
                    continue;

                //There is a match and will add the shader stage to the first push range of the match
                for (auto& pushRange : pushConstantRanges) {
                    if (pushRange.stageFlags == shaderStage) {
                        pushRange.stageFlags |= stage;
                        pushMemberData[stage] = pushData;
                        return;
                    }
                }

            }
        }

        VkPushConstantRange pushConstantRange = {};
        pushConstantRange.size = bufferSize - pushData[0].offset;
        pushConstantRange.stageFlags = stage;
        pushConstantRange.offset = pushData[0].offset;

        for (int i = 0; i < pushData.size(); i++) {
           std::string name = compiler->get_member_name(resource.base_type_id, i);
           uniforms[name] = pushData[i];
        }

        pushMemberData[stage] = pushData;

        pushConstantRanges.push_back(pushConstantRange);
    }

    // Uniform Buffers
    for (auto& resource : resources.uniform_buffers) {
        auto bufferType = compiler->get_type(resource.base_type_id);

        for (int i = 0; i < bufferType.member_types.size(); i++) {
            LOGGER_TRACE(compiler->get_member_name(resource.base_type_id, i));
        }
    }

}
