find_package(GTest)

if(NOT GTest_FOUND AND NOT GTEST_FOUND)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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
