add_library(dataconfigparser "")
target_sources(dataconfigparser
    PRIVATE
        dataconfigparser.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/dataconfigparser.h
)

target_link_libraries(dataconfigparser PUBLIC
    ghc_filesystem
    fmt::fmt
)

target_include_directories(dataconfigparser PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(dataconfigparser PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(dataconfigparser PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
