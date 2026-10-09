if(WIN32)
    # Search for pthread header and library
    find_path(PTHREAD_INCLUDE_DIR pthread.h)
    find_library(PTHREAD_LIBRARY NAMES pthreadVC2 pthreadVC3 pthread)

    # Search for dlfcn-win32 header and library
    find_path(DLFCN_INCLUDE_DIR dlfcn.h)
    find_library(DLFCN_LIBRARY NAMES dl dlfcn)
endif()

file(GLOB SOURCES "*.cpp")

add_library(nds3 SHARED ${SOURCES})
add_library(nds3_static STATIC ${SOURCES})
set_target_properties(nds3_static PROPERTIES OUTPUT_NAME nds3)

if(WIN32)
    # The static library and the import library of the DLL are both nds3.lib,
    # as the EPICS build system expects, so they need separate directories:
    # in the build tree as well as when installed.
    set_target_properties(nds3_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/static)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(NDS3_DEFAULT_EPICS_ARCH windows-x64)
    else()
        set(NDS3_DEFAULT_EPICS_ARCH win32-x86)
    endif()
    set(NDS3_EPICS_ARCH ${NDS3_DEFAULT_EPICS_ARCH} CACHE STRING
        "EPICS architecture naming the bin/ and lib/ install subdirectories on Windows")
endif()

target_include_directories(nds3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(nds3_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WIN32)
    # No installed header names a pthread or dlfcn type, so neither location has
    # to propagate to anything using NDS3.
    if(PTHREAD_INCLUDE_DIR)
        target_include_directories(nds3 PRIVATE ${PTHREAD_INCLUDE_DIR})
        target_include_directories(nds3_static PRIVATE ${PTHREAD_INCLUDE_DIR})
    endif()
    if(DLFCN_INCLUDE_DIR)
        target_include_directories(nds3 PRIVATE ${DLFCN_INCLUDE_DIR})
        target_include_directories(nds3_static PRIVATE ${DLFCN_INCLUDE_DIR})
    endif()
endif()

# Only the DLL gets these: for the static library NDS3_API must stay empty, or
# its users would import from a DLL that is not there.
target_compile_definitions(nds3 PUBLIC NDS3_DLL PRIVATE NDS3_DLL_EXPORTS)

target_link_libraries(nds3 PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
target_link_libraries(nds3_static PUBLIC Threads::Threads ${CMAKE_DL_LIBS})

if(WIN32)
    target_link_libraries(nds3 PUBLIC ${PTHREAD_LIBRARY} ${DLFCN_LIBRARY})
    target_link_libraries(nds3_static PUBLIC ${PTHREAD_LIBRARY} ${DLFCN_LIBRARY})
endif()

if(NOT MSVC)
    target_compile_options(nds3 PRIVATE -Wall -Wextra -pedantic -fvisibility-inlines-hidden)
    target_compile_options(nds3_static PRIVATE -Wall -Wextra -pedantic -fvisibility-inlines-hidden)
endif()

if(WIN32)
    # Install in the layout of an EPICS module, so that EPICS-based modules can
    # build against NDS3 like against any other module. As in EPICS, a debug
    # build is a separate architecture, so it does not overwrite the release.
    set(NDS3_DEBUG_SUFFIX $<$<CONFIG:Debug>:-debug>)
    install(TARGETS nds3
        EXPORT nds3Targets
        ARCHIVE DESTINATION lib/${NDS3_EPICS_ARCH}${NDS3_DEBUG_SUFFIX}
        RUNTIME DESTINATION bin/${NDS3_EPICS_ARCH}${NDS3_DEBUG_SUFFIX}
    )
    install(TARGETS nds3_static
        EXPORT nds3Targets
        ARCHIVE DESTINATION lib/${NDS3_EPICS_ARCH}-static${NDS3_DEBUG_SUFFIX}
    )
else()
    install(TARGETS nds3 nds3_static
        EXPORT nds3Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nds3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
