add_executable(runner_test test.cc)

target_link_libraries(runner_test PRIVATE 
    catch
    ghc_filesystem
    gen_test
    runner
)

configure_file(tests/aplusb.cpp aplusb.cpp COPYONLY)
configure_file(tests/errorfile.cpp errorfile.cpp COPYONLY)
configure_file(tests/extracommand.cpp extracommand.cpp COPYONLY)

add_test(runner runner_test)
