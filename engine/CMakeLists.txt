find_package(Vulkan REQUIRED)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
	message(STATUS Vulkan::Vulkan)
ENDIF()

add_definitions(-DASSETS_PATH=\"${ASSET_PATH_DIR}\")

set(LIB_NAME "lib${PROJECT_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/core/application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/GameObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/gf3d_camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/gf3d_logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/memory/LinearAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/platform/gf3d_window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_device.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_material_system.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_pipeline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_swapchain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/gf3d_validations.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/vulkan_functions.cpp
)

add_library("${LIB_NAME}" "${SOURCE_FILES}")

target_include_directories("${LIB_NAME}" PRIVATE "${gf3d_INCLUDE_PATHS}")

target_link_libraries("${LIB_NAME}" glfw)
target_link_libraries("${LIB_NAME}" Vulkan::Vulkan)

set_target_properties("${LIB_NAME}" PROPERTIES PREFIX "")

if(WIN32)
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/spirv-cross-cored.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/spirv-cross-glsld.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/glslangd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/HLSLd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/OSDependentd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/MachineIndependentd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/GenericCodeGend.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/OGLCompilerd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/SPIRVd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib")
	target_link_libraries("${LIB_NAME}" debug "$ENV{VULKAN_SDK}/Lib/SPIRV-Tools-optd.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/spirv-cross-core.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/spirv-cross-glsl.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/glslang.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/HLSL.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/OSDependent.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/MachineIndependent.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/GenericCodeGen.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/OGLCompiler.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/SPIRV.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/SPIRV-Tools.lib")
	target_link_libraries("${LIB_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/SPIRV-Tools-opt.lib")
elseif(APPLE)
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libspirv-cross-core.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libspirv-cross-glsl.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libglslang.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libHLSL.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libOSDependent.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libMachineIndependent.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libGenericCodeGen.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libOGLCompiler.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libspirv.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libspirv-Tools.a")
	target_link_libraries("${LIB_NAME}" "/usr/local/lib/libspirv-Tools-opt.a")
endif()