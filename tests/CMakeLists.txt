include(GoogleTest)
include("${PROJECT_SOURCE_DIR}/cmake/package_add_test.cmake")

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" extern/googletest)

package_add_test(test_file test_file_io.cpp)
