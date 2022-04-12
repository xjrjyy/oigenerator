if(WIN32)
else()
    if(APPLE)
        # TODO: watcher_macos
    else()
        add_executable(watcher_unix watcher_linux.cpp)
    endif()
endif()

target_link_libraries(watcher_unix PUBLIC
    ghc_filesystem
    fmt::fmt
)

target_include_directories(watcher_unix PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(watcher_unix PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(watcher_unix PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
