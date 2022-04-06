add_library(configparser "")
target_sources(configparser
    PRIVATE
    configparser.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/configparser.h
)

target_link_libraries(configparser PUBLIC
    ghc_filesystem
    fmt::fmt
)

target_include_directories(configparser PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(configparser PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(configparser PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
