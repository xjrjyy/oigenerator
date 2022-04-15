add_library(config "")
target_sources(config
    PRIVATE
        config.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/config.h
)

target_include_directories(config PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(config PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(config PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
