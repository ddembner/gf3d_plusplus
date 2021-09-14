set(TESTS_PROJECT "tests")

set(tests_SOURCE_FILES
	"src/tests_main.cpp"
	"src/test_manager.cpp"
	"src/test_linear_allocator.cpp"
)

set(tests_INCLUDE_PATHS
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${gf3d_INCLUDE_PATHS}"
)

add_executable("${TESTS_PROJECT}" "${tests_SOURCE_FILES}")

target_link_libraries("${TESTS_PROJECT}" "libgf3d++")

target_include_directories("${TESTS_PROJECT}" PRIVATE "${tests_INCLUDE_PATHS}")