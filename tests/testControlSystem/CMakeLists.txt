file(GLOB TCS_SOURCES "*.cpp")
add_library(nds3-testcontrolsystem SHARED ${TCS_SOURCES})
target_link_libraries(nds3-testcontrolsystem PUBLIC nds3)
target_include_directories(nds3-testcontrolsystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

if(NOT MSVC)
    target_compile_options(nds3-testcontrolsystem PRIVATE -Wall -Wextra -pedantic)
endif()
