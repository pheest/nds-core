add_library(nds3-dev-nds-example SHARED nds-example.cpp)
target_link_libraries(nds3-dev-nds-example PRIVATE nds3)
target_compile_definitions(nds3-dev-nds-example PRIVATE NDS3_DLL)

if(NOT MSVC)
    target_compile_options(nds3-dev-nds-example PRIVATE -Wall -Wextra -pedantic)
endif()
