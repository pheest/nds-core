add_library(simulated-signal STATIC simDev/simulated_signal.cpp)
set_target_properties(simulated-signal PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(simulated-signal PRIVATE simDev)

file(GLOB DRV_SOURCES "ndsDrv/*.cpp")
add_library(nds3-exampleDrv SHARED ${DRV_SOURCES})
target_link_libraries(nds3-exampleDrv PRIVATE nds3 simulated-signal)
target_include_directories(nds3-exampleDrv PRIVATE simDev)
target_compile_definitions(nds3-exampleDrv PRIVATE EPICS NDS3_DLL)

if(NOT MSVC)
    target_compile_options(nds3-exampleDrv PRIVATE -Wall -Wextra -pedantic)
endif()
