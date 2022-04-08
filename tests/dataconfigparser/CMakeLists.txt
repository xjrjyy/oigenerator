add_executable(dataconfigparser_test test.cc)

target_link_libraries(dataconfigparser_test PRIVATE 
    catch
    ghc_filesystem
    gen_test
    dataconfigparser
)

add_test(dataconfigparser dataconfigparser_test)
