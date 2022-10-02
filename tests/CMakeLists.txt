include(GoogleTest)
include("${PROJECT_SOURCE_DIR}/cmake/package_add_test.cmake")

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" extern/googletest)

# sanitizer configuration
set(ASAN_CXX_FLAGS  "-O0;-fsanitize=address;-fno-omit-frame-pointer;-fsanitize-address-use-after-scope")
set(ASAN_LINK_FLAGS "-fsanitize=address")

package_add_test(test_error
    FILES     test_error.cpp
    LIBRARIES sys)

package_add_test(test_fileio
    FILES     test_fileio.cpp
    LIBRARIES sys)

package_add_test(test_utils
    FILES     test_utils.cpp
    LIBRARIES sys)
