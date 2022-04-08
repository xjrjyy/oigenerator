add_library(gen_test STATIC EXCLUDE_FROM_ALL main.cc)

target_link_libraries(gen_test PRIVATE catch)

add_subdirectory(dataconfigparser)
add_subdirectory(comparer)
