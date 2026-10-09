find_package(GTest)

if(NOT GTest_FOUND AND NOT GTEST_FOUND)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # GoogleTest requires only CMake 3.13, which would leave it on the old way
    # of setting the MSVC runtime: through flags this project no longer has, so
    # it would end up with the static runtime instead of the DLL one.
    set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
    FetchContent_MakeAvailable(googletest)

    if(NOT TARGET GTest::GTest)
        add_library(GTest::GTest ALIAS gtest)
    endif()
    set(GTest_FOUND ON)
    set(GTEST_FOUND ON)
endif()

add_subdirectory(testControlSystem)
add_subdirectory(unit)
add_subdirectory(unit-example)
