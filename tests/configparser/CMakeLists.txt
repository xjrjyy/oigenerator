add_executable(configparser_test test.cc)

target_link_libraries(configparser_test PRIVATE 
    catch
    ghc_filesystem
    gen_test
    configparser
)

add_test(configparser configparser_test)
