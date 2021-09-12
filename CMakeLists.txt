cmake_minimum_required(VERSION 3.15.0)

project(gf3d++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ASSET_PATH_DIR "${CMAKE_SOURCE_DIR}/assets/")

set(gf3d_INCLUDE_PATHS
	${CMAKE_CURRENT_SOURCE_DIR}/engine
	${CMAKE_SOURCE_DIR}/third_party/glfw/include
	${CMAKE_SOURCE_DIR}/third_party/glm
	${CMAKE_SOURCE_DIR}/third_party/vma/include
	${CMAKE_SOURCE_DIR}/third_party/spdlog/include
)

add_subdirectory(third_party/glfw)
add_subdirectory(engine)
add_subdirectory(application)

if(WIN32)
	#TODO: make application the default startup
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
endif()