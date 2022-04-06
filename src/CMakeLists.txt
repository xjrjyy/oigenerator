add_executable(oigen "")
target_sources(oigen
    PRIVATE
        config.cc
        main.cc
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/config.h
)

add_subdirectory(configparser)
add_subdirectory(compiler)
add_subdirectory(runner)

target_link_libraries(oigen PRIVATE
    configparser
    compiler
    runner
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
