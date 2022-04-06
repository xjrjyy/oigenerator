add_executable(compiler_test test.cc)

target_link_libraries(compiler_test PRIVATE 
    catch
    gen_test
    compiler
)

add_test(compiler compiler_test)
