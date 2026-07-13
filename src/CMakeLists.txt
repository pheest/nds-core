if(WIN32)
    # Search for pthread header and library
    find_path(PTHREAD_INCLUDE_DIR pthread.h)
    find_library(PTHREAD_LIBRARY NAMES pthreadVC2 pthreadVC3 pthread)

    # Search for dlfcn-win32 header and library
    find_path(DLFCN_INCLUDE_DIR dlfcn.h)
    find_library(DLFCN_LIBRARY NAMES dl dlfcn)

    if(PTHREAD_INCLUDE_DIR)
        include_directories(${PTHREAD_INCLUDE_DIR})
    endif()
    if(DLFCN_INCLUDE_DIR)
        include_directories(${DLFCN_INCLUDE_DIR})
    endif()
endif()

file(GLOB SOURCES "*.cpp")

add_library(nds3 SHARED ${SOURCES})
add_library(nds3_static STATIC ${SOURCES})
set_target_properties(nds3_static PROPERTIES OUTPUT_NAME nds3)

target_include_directories(nds3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(nds3_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(nds3 PUBLIC NDS3_DLL PRIVATE NDS3_DLL_EXPORTS)
target_compile_definitions(nds3_static PUBLIC NDS3_DLL PRIVATE NDS3_DLL_EXPORTS)

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

install(TARGETS nds3 nds3_static
    EXPORT nds3Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nds3 DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
