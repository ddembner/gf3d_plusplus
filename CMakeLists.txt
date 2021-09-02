cmake_minimum_required(VERSION 3.15.0)

project(gf3d++)

find_package(Vulkan REQUIRED)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
	message(STATUS Vulkan::Vulkan)
ENDIF()

add_subdirectory(./external/glfw)
add_subdirectory(./external/spirv-cross)

set(ASSET_PATH_DIR "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSETS_PATH=\"${ASSET_PATH_DIR}\")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_device.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_material_system.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_pipeline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_swapchain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_validations.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vector2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vector3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan_functions.cpp
)

set(INCLUDE_PATHS
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/glm
	${CMAKE_CURRENT_SOURCE_DIR}/external/vma/src
	${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog/include
)

add_executable("${PROJECT_NAME}" "${SOURCE_FILES}")

target_include_directories("${PROJECT_NAME}" PRIVATE "${INCLUDE_PATHS}")

target_link_libraries("${PROJECT_NAME}" glfw)
target_link_libraries("${PROJECT_NAME}" spirv-cross-glsl)
target_link_libraries("${PROJECT_NAME}" Vulkan::Vulkan)

if(WIN32)
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/glslangd.lib")
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/HLSLd.lib")
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/OSDependentd.lib")
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/MachineIndependentd.lib")
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/GenericCodeGend.lib")
	target_link_libraries("${PROJECT_NAME}" debug "$ENV{VULKAN_SDK}/Lib/OGLCompilerd.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/glslang.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/HLSL.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/OSDependent.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/MachineIndependent.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/GenericCodeGen.lib")
	target_link_libraries("${PROJECT_NAME}" optimized "$ENV{VULKAN_SDK}/Lib/OGLCompiler.lib")
endif()

if(WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()