if(WIN32)
    # Search for pthread header and library. Without path specification, it finds the wrong ones.
    find_path(PTHREAD_INCLUDE_DIR pthread.h)
    find_library(PTHREAD_LIBRARY_DLL NAMES pthreadVC2 pthreadVC3 pthread PATHS "$ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows/lib" NO_DEFAULT_PATH)
    find_library(PTHREAD_LIBRARY_LIB NAMES pthreadVC2 pthreadVC3 pthread PATHS "$ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows-static/lib" NO_DEFAULT_PATH)

    # Search for dlfcn-win32 header and library. Without path specification, it finds the wrong ones.
    find_path(DLFCN_INCLUDE_DIR dlfcn.h)
    find_library(DLFCN_LIBRARY_DLL NAMES dl dlfcn PATHS "$ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows/lib" NO_DEFAULT_PATH)
    find_library(DLFCN_LIBRARY_LIB NAMES dl dlfcn PATHS "$ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows-static/lib" NO_DEFAULT_PATH)
endif()

file(GLOB SOURCES "*.cpp")

add_library(nds3 SHARED ${SOURCES})
if (WIN32)
    # see https://gitlab.kitware.com/cmake/cmake/-/work_items/16931
    add_library(imported_libs OBJECT IMPORTED)
    set_property(TARGET imported_libs PROPERTY IMPORTED_OBJECTS ${PTHREAD_LIBRARY_LIB} ${DLFCN_LIBRARY_LIB})
    add_library(nds3_static STATIC ${SOURCES} $<TARGET_OBJECTS:imported_libs>)
else()
    add_library(nds3_static STATIC ${SOURCES})
endif()

set_target_properties(nds3_static PROPERTIES OUTPUT_NAME nds3)

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

target_compile_definitions(nds3 PUBLIC NDS3_DLL PRIVATE NDS3_DLL_EXPORTS)

target_link_libraries(nds3 PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
target_link_libraries(nds3_static PUBLIC Threads::Threads ${CMAKE_DL_LIBS})

if(WIN32)
    target_link_libraries(nds3 PUBLIC ${PTHREAD_LIBRARY_DLL} ${DLFCN_LIBRARY_DLL})
    set_target_properties(nds3 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib/windows-x64" RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/windows-x64")
    set_target_properties(nds3_static PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_target_properties(nds3_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib/windows-x64-static")
    install(TARGETS nds3 nds3_static
        EXPORT nds3Targets
    )
else()
    target_compile_options(nds3 PRIVATE -Wall -Wextra -pedantic -fvisibility-inlines-hidden)
    target_compile_options(nds3_static PRIVATE -Wall -Wextra -pedantic -fvisibility-inlines-hidden)
    install(TARGETS nds3 nds3_static
        EXPORT nds3Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nds3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
