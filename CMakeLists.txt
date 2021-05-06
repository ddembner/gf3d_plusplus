cmake_minimum_required(VERSION 3.15.0)

project(gf3d++)

find_package(Vulkan REQUIRED)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

add_subdirectory(./external/glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_device.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gf3d_pipeline.cpp
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
target_link_libraries("${PROJECT_NAME}" Vulkan::Vulkan)

if(WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
endif()