add_executable(oigen main.cc)

add_subdirectory(config)
add_subdirectory(dataconfigparser)
add_subdirectory(compiler)
add_subdirectory(runner)
add_subdirectory(comparer)

target_link_libraries(oigen PRIVATE
    dataconfigparser
    compiler
    runner
    linebylinecomparer
    fmt::fmt
    cxxopts
)

target_include_directories(oigen PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(oigen PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(oigen PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
