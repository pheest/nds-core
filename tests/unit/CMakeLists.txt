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

    # All the device modules are built into one directory. Tell the test where it
    # is instead of having it guess with relative paths, which cannot account for
    # the per-configuration subdirectory a multi-configuration generator adds.
    list(GET NODE_LIBS 0 FIRST_NODE_LIB)
    target_compile_definitions(nds3-unit-tests PRIVATE
        "NDS3_TEST_MODULE_DIR=\"$<TARGET_FILE_DIR:${FIRST_NODE_LIB}>\"")

    add_test(NAME nds3-unit-tests COMMAND nds3-unit-tests)

    if(WIN32)
        # Windows has no RPATH: the DLLs this executable links against are built
        # into sibling directories, so they have to be on PATH or the process
        # cannot start. (ENVIRONMENT_MODIFICATION needs CMake 3.22 or later; on
        # older versions the property is simply ignored.)
        set_tests_properties(nds3-unit-tests PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:nds3>;PATH=path_list_prepend:$<TARGET_FILE_DIR:nds3-testcontrolsystem>;PATH=path_list_prepend:$<TARGET_FILE_DIR:${FIRST_NODE_LIB}>")
    endif()
endif()
