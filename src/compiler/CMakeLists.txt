add_library(compiler "")
target_sources(compiler
    PRIVATE
        compiler.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/compiler.h
)

target_link_libraries(compiler PUBLIC
    ghc_filesystem
    fmt::fmt
    config
)

target_include_directories(compiler PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(compiler PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(compiler PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
