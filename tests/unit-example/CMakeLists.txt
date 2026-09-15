if(GTest_FOUND OR GTEST_FOUND)
    file(GLOB UNIT_EXAMPLE_SOURCES "*.cpp")
    add_executable(nds3-unit-example ${UNIT_EXAMPLE_SOURCES})

    target_link_libraries(nds3-unit-example PRIVATE
        nds3
        nds3-dev-nds-example
        nds3-testcontrolsystem
        GTest::GTest
    )

    target_include_directories(nds3-unit-example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/examples/nds-example
    )

    add_test(NAME nds3-unit-example COMMAND nds3-unit-example)

    if(WIN32)
        # See the comment in tests/unit/CMakeLists.txt.
        set_tests_properties(nds3-unit-example PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:nds3>;PATH=path_list_prepend:$<TARGET_FILE_DIR:nds3-testcontrolsystem>;PATH=path_list_prepend:$<TARGET_FILE_DIR:nds3-dev-nds-example>")
    endif()
endif()
