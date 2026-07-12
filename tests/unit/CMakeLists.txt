if(GTest_FOUND OR GTEST_FOUND)
    file(GLOB UNIT_SOURCES "*.cpp")
    add_executable(nds3-unit-tests ${UNIT_SOURCES})

    file(GLOB NODE_SOURCES "${CMAKE_SOURCE_DIR}/examples/nodesTest/*.cpp")
    set(NODE_LIBS "")
    foreach(SOURCE ${NODE_SOURCES})
        get_filename_component(NAME ${SOURCE} NAME_WE)
        list(APPEND NODE_LIBS nds3-${NAME})
    endforeach()

    target_link_libraries(nds3-unit-tests PRIVATE
        nds3
        nds3-testcontrolsystem
        GTest::GTest
        ${NODE_LIBS}
    )

    target_include_directories(nds3-unit-tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/examples/nodesTest
    )

    add_test(NAME nds3-unit-tests COMMAND nds3-unit-tests)
endif()
