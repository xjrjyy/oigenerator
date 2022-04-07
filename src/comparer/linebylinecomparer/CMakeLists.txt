add_library(linebylinecomparer "")
target_sources(linebylinecomparer
    PRIVATE
        linebylinecomparer.cc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/linebylinecomparer.h
)

target_link_libraries(linebylinecomparer PUBLIC
    ghc_filesystem
)

target_include_directories(linebylinecomparer PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(linebylinecomparer PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(linebylinecomparer PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
