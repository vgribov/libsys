include(GoogleTest)
include("${PROJECT_SOURCE_DIR}/cmake/package_add_test.cmake")

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" extern/googletest)

# sanitizer configuration
set(ASAN_CXX_FLAGS  "-O0;-fsanitize=address;-fno-omit-frame-pointer;-fsanitize-address-use-after-scope")
set(ASAN_LINK_FLAGS "-fsanitize=address")

package_add_test_with_libraries(test_fileio test_fileio.cpp sys "${PROJECT_DIR}")
