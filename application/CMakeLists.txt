cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp
)

add_executable("${PROJECT_NAME}" "${SOURCE_FILES}")

target_include_directories("${PROJECT_NAME}" PRIVATE "${gf3d_INCLUDE_PATHS}")
target_link_libraries("${PROJECT_NAME}" "libgf3d++")

if(UNIX OR APPLE)
	set_target_properties("${PROJECT_NAME}" PROPERTIES SUFFIX ".out")
endif()

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()