add_library(runner "")
target_sources(runner
    PRIVATE
        runner.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/runner.h
)

target_link_libraries(runner PUBLIC
    ghc_filesystem
    fmt::fmt
    compiler
)

target_include_directories(runner PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(runner PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(runner PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
