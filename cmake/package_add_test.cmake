function(package_add_test TESTNAME)
    cmake_parse_arguments(ARG                   # arguments prefix
                          ""                    # boolean arguments
                          "WORKDIR"             # single value arguments
                          "FILES;LIBRARIES"     # multi value arguments
                          ${ARGN})

    if (NOT DEFINED ARG_WORKDIR)
        set(ARG_WORKDIR "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    add_executable(${TESTNAME} ${ARG_FILES})

    target_link_libraries (${TESTNAME} gtest gmock gtest_main ${ARG_LIBRARIES})
    target_compile_options(${TESTNAME} PRIVATE "$<$<CONFIG:Debug>:${ASAN_CXX_FLAGS}>")
    target_link_options   (${TESTNAME} PRIVATE "$<$<CONFIG:Debug>:${ASAN_LINK_FLAGS}>")

    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${ARG_WORKDIR})

    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endfunction()
