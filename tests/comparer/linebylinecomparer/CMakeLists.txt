add_executable(linebylinecomparer_test test.cc)

target_link_libraries(linebylinecomparer_test PRIVATE 
    catch
    ghc_filesystem
    gen_test
    linebylinecomparer
)

add_test(linebylinecomparer linebylinecomparer_test)
