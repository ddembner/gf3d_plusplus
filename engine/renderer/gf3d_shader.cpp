#include "gf3d_shader.h"
#include "vulkan_functions.h"
#include "core/gf3d_logger.h"
#include <glslang/SPIRV/GlslangToSpv.h>
#include <spirv_cross/spirv_glsl.hpp>

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
        case VK_SHADER_STAGE_VERTEX_BIT: return EShLangVertex;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShLangTessControl;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShLangTessEvaluation;
        case VK_SHADER_STAGE_GEOMETRY_BIT: return EShLangGeometry;
        case VK_SHADER_STAGE_FRAGMENT_BIT: return EShLangFragment;
        case VK_SHADER_STAGE_COMPUTE_BIT: return EShLangCompute;
        default: return EShLangVertex;
    }
}

u32 Shader::getPushDataSize()
{
    u32 total = 0;
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

static TBuiltInResource InitResources()
{
    TBuiltInResource Resources;

    Resources.maxLights = 32;
    Resources.maxClipPlanes = 6;
    Resources.maxTextureUnits = 32;
    Resources.maxTextureCoords = 32;
    Resources.maxVertexAttribs = 64;
    Resources.maxVertexUniformComponents = 4096;
    Resources.maxVaryingFloats = 64;
    Resources.maxVertexTextureImageUnits = 32;
    Resources.maxCombinedTextureImageUnits = 80;
    Resources.maxTextureImageUnits = 32;
    Resources.maxFragmentUniformComponents = 4096;
    Resources.maxDrawBuffers = 32;
    Resources.maxVertexUniformVectors = 128;
    Resources.maxVaryingVectors = 8;
    Resources.maxFragmentUniformVectors = 16;
    Resources.maxVertexOutputVectors = 16;
    Resources.maxFragmentInputVectors = 15;
    Resources.minProgramTexelOffset = -8;
    Resources.maxProgramTexelOffset = 7;
    Resources.maxClipDistances = 8;
    Resources.maxComputeWorkGroupCountX = 65535;
    Resources.maxComputeWorkGroupCountY = 65535;
    Resources.maxComputeWorkGroupCountZ = 65535;
    Resources.maxComputeWorkGroupSizeX = 1024;
    Resources.maxComputeWorkGroupSizeY = 1024;
    Resources.maxComputeWorkGroupSizeZ = 64;
    Resources.maxComputeUniformComponents = 1024;
    Resources.maxComputeTextureImageUnits = 16;
    Resources.maxComputeImageUniforms = 8;
    Resources.maxComputeAtomicCounters = 8;
    Resources.maxComputeAtomicCounterBuffers = 1;
    Resources.maxVaryingComponents = 60;
    Resources.maxVertexOutputComponents = 64;
    Resources.maxGeometryInputComponents = 64;
    Resources.maxGeometryOutputComponents = 128;
    Resources.maxFragmentInputComponents = 128;
    Resources.maxImageUnits = 8;
    Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    Resources.maxCombinedShaderOutputResources = 8;
    Resources.maxImageSamples = 0;
    Resources.maxVertexImageUniforms = 0;
    Resources.maxTessControlImageUniforms = 0;
    Resources.maxTessEvaluationImageUniforms = 0;
    Resources.maxGeometryImageUniforms = 0;
    Resources.maxFragmentImageUniforms = 8;
    Resources.maxCombinedImageUniforms = 8;
    Resources.maxGeometryTextureImageUnits = 16;
    Resources.maxGeometryOutputVertices = 256;
    Resources.maxGeometryTotalOutputComponents = 1024;
    Resources.maxGeometryUniformComponents = 1024;
    Resources.maxGeometryVaryingComponents = 64;
    Resources.maxTessControlInputComponents = 128;
    Resources.maxTessControlOutputComponents = 128;
    Resources.maxTessControlTextureImageUnits = 16;
    Resources.maxTessControlUniformComponents = 1024;
    Resources.maxTessControlTotalOutputComponents = 4096;
    Resources.maxTessEvaluationInputComponents = 128;
    Resources.maxTessEvaluationOutputComponents = 128;
    Resources.maxTessEvaluationTextureImageUnits = 16;
    Resources.maxTessEvaluationUniformComponents = 1024;
    Resources.maxTessPatchComponents = 120;
    Resources.maxPatchVertices = 32;
    Resources.maxTessGenLevel = 64;
    Resources.maxViewports = 16;
    Resources.maxVertexAtomicCounters = 0;
    Resources.maxTessControlAtomicCounters = 0;
    Resources.maxTessEvaluationAtomicCounters = 0;
    Resources.maxGeometryAtomicCounters = 0;
    Resources.maxFragmentAtomicCounters = 8;
    Resources.maxCombinedAtomicCounters = 8;
    Resources.maxAtomicCounterBindings = 1;
    Resources.maxVertexAtomicCounterBuffers = 0;
    Resources.maxTessControlAtomicCounterBuffers = 0;
    Resources.maxTessEvaluationAtomicCounterBuffers = 0;
    Resources.maxGeometryAtomicCounterBuffers = 0;
    Resources.maxFragmentAtomicCounterBuffers = 1;
    Resources.maxCombinedAtomicCounterBuffers = 1;
    Resources.maxAtomicCounterBufferSize = 16384;
    Resources.maxTransformFeedbackBuffers = 4;
    Resources.maxTransformFeedbackInterleavedComponents = 64;
    Resources.maxCullDistances = 8;
    Resources.maxCombinedClipAndCullDistances = 8;
    Resources.maxSamples = 4;
    Resources.maxMeshOutputVerticesNV = 256;
    Resources.maxMeshOutputPrimitivesNV = 512;
    Resources.maxMeshWorkGroupSizeX_NV = 32;
    Resources.maxMeshWorkGroupSizeY_NV = 1;
    Resources.maxMeshWorkGroupSizeZ_NV = 1;
    Resources.maxTaskWorkGroupSizeX_NV = 32;
    Resources.maxTaskWorkGroupSizeY_NV = 1;
    Resources.maxTaskWorkGroupSizeZ_NV = 1;
    Resources.maxMeshViewCountNV = 4;

    Resources.limits.nonInductiveForLoops = 1;
    Resources.limits.whileLoops = 1;
    Resources.limits.doWhileLoops = 1;
    Resources.limits.generalUniformIndexing = 1;
    Resources.limits.generalAttributeMatrixVectorIndexing = 1;
    Resources.limits.generalVaryingIndexing = 1;
    Resources.limits.generalSamplerIndexing = 1;
    Resources.limits.generalVariableIndexing = 1;
    Resources.limits.generalConstantMatrixVectorIndexing = 1;

    return Resources;
}

void Shader::readPreCompiledFiles()
{
    for (auto&& [stage, source] : shaderSources) {
        std::ifstream in(getShaderFileFinalNameForStage(stage), std::ios::in | std::ios::binary);

        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        in.seekg(0, std::ios::beg);

        gf3d::vector<u32> data(size / sizeof(u32));
        in.read((char*)data.data(), size);
        
        Reflect(stage, data);

        loadShaderModule(stage, data);
    }
}

void Shader::compileShadersToSpv()
{
    for (auto&& [stage, source] : shaderSources) {
        
        gf3d::vector shaderData = compileSourceToSpirv(stage, source);
         
        Reflect(stage, shaderData);

        loadShaderModule(stage, shaderData);
    }
}

gf3d::vector<u32> Shader::compileSourceToSpirv(VkShaderStageFlagBits stage, const std::string& source)
{
    glslang::InitializeProcess();
    EShLanguage glslangStage = shaderStageToGlslangKind(stage);
    glslang::TShader shader(glslangStage);
    glslang::TProgram program;
    TBuiltInResource  resources = InitResources();
    
    const char* shaderCode = source.c_str();
    shader.setStrings(&shaderCode, 1);
    shader.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_3);
    shader.setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EShTargetClientVersion::EShTargetVulkan_1_2);

    EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

    if (!shader.parse(&resources, 100, false, messages)) {
        LOGGER_WARN(shader.getInfoLog());
        LOGGER_WARN(shader.getInfoDebugLog());
        return gf3d::vector<u32>(0);
    }

    program.addShader(&shader);

    if (!program.link(messages)) {
        LOGGER_WARN(shader.getInfoLog());
        LOGGER_WARN(shader.getInfoDebugLog());
        return gf3d::vector<u32>(0);
    }

    std::vector<u32> shaderData;
    glslang::GlslangToSpv(*program.getIntermediate(glslangStage), shaderData);

    std::ofstream out(getShaderFileFinalNameForStage(stage), std::ios::out | std::ios::binary);
    if (out.is_open()) {
        out.write((char*)shaderData.data(), shaderData.size() * sizeof(u32));
        out.flush();
        out.close();
    }

    glslang::FinalizeProcess();

    gf3d::vector<u32> _shaderData(shaderData.size());

    for (u64 i = 0; i < _shaderData.size(); i++) {
        _shaderData[i] = shaderData[i];
    }

    return _shaderData;
}

void Shader::loadShaderModule(VkShaderStageFlagBits stage, const gf3d::vector<u32>& codeData)
{
    VkShaderModuleCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    shaderInfo.codeSize = codeData.size() * sizeof(u32);
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

void Shader::Reflect(VkShaderStageFlagBits stage, gf3d::vector<u32>& data)
{
    std::vector<u32> _data(data.size());
    for (u64 i = 0; i < _data.size(); i++) {
        _data[i] = data[i];
    }


    std::unique_ptr<spirv_cross::CompilerGLSL> compiler = std::make_unique<spirv_cross::CompilerGLSL>(_data);

    spirv_cross::CompilerGLSL::Options options;
    options.vulkan_semantics = true;
    compiler->set_common_options(options);
    spirv_cross::ShaderResources resources = compiler->get_shader_resources();

    // Push Constants
    for (auto& resource : resources.push_constant_buffers) {
        
        const auto& bufferType = compiler->get_type(resource.base_type_id);
        u32 bufferSize = compiler->get_declared_struct_size(bufferType);
        gf3d::vector<Uniform> pushData(bufferType.member_types.size());

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

    compiler.release();
}
