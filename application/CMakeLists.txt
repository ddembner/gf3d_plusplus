cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp
)

set(INCLUDE_PATHS
	${CMAKE_SOURCE_DIR}/engine
	${CMAKE_SOURCE_DIR}/engine/third_party/glfw/include
	${CMAKE_SOURCE_DIR}/engine/third_party/glm
	${CMAKE_SOURCE_DIR}/engine/third_party/vma/src
	${CMAKE_SOURCE_DIR}/engine/third_party/spdlog/include
)

add_executable("${PROJECT_NAME}" "${SOURCE_FILES}")

target_include_directories("${PROJECT_NAME}" PRIVATE "${INCLUDE_PATHS}")
target_link_libraries("${PROJECT_NAME}" "libgf3d++")

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()