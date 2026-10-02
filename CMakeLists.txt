# See https://stackoverflow.com/questions/78430503/setting-property-msvc-runtime-library-doesnt-change-the-runtime-library
cmake_minimum_required(VERSION 3.15)
project(nds3 VERSION 3.3.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

find_package(Threads REQUIRED)

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
