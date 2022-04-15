#include "catch.hpp"

#include <string>
#include <vector>

#include "filesystem.h"

#include "runner.h"

TEST_CASE("test runner") {
    Config config;
    const fs::path input_path(fs::temp_directory_path() / "test_oigen_runner_1.in");
    const fs::path output_path(fs::temp_directory_path() / "test_oigen_runner_1.out");
    const fs::path error_path(fs::temp_directory_path() / "test_oigen_runner_1.err");
    SECTION("test a+b problem") {
        fs::path aplusb_cpp_path("aplusb.cpp");
        REQUIRE(fs::exists(aplusb_cpp_path));
        Runner runner(aplusb_cpp_path, config);
        fs::ofstream input_file;
        fs::ifstream output_file;
        std::string line;

        input_file.open(input_path, std::ios::binary);
        REQUIRE(input_file.is_open());
        input_file << "2 3\n";
        input_file.close();
        REQUIRE(runner.Start(input_path, output_path) == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "5");
        REQUIRE(output_file.eof());
        output_file.close();

        input_file.open(input_path, std::ios::binary);
        REQUIRE(input_file.is_open());
        input_file << "1000000000 1000000000\n";
        input_file.close();
        REQUIRE(runner.Start(input_path, output_path) == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "2000000000");
        REQUIRE(output_file.eof());
        output_file.close();
    }
    SECTION("test error file") {
        fs::path aplusb_cpp_path("errorfile.cpp");
        REQUIRE(fs::exists(aplusb_cpp_path));
        Runner runner(aplusb_cpp_path, config);
        fs::ofstream input_file;
        fs::ifstream output_file;
        fs::ifstream error_file;
        std::string line;

        input_file.open(input_path, std::ios::binary);
        REQUIRE(input_file.is_open());
        input_file << "2 3\n";
        input_file.close();
        REQUIRE(runner.Start(input_path, output_path, error_path) == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "5");
        REQUIRE(output_file.eof());
        output_file.close();
        error_file.open(error_path, std::ios::binary);
        REQUIRE(error_file.is_open());
        std::getline(error_file, line);
        REQUIRE(line == "-1");
        REQUIRE(error_file.eof());
        error_file.close();

        input_file.open(input_path, std::ios::binary);
        REQUIRE(input_file.is_open());
        input_file << "12345678 87654321\n";
        input_file.close();
        REQUIRE(runner.Start(input_path, output_path, error_path) == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "99999999");
        REQUIRE(output_file.eof());
        output_file.close();
        error_file.open(error_path, std::ios::binary);
        REQUIRE(error_file.is_open());
        std::getline(error_file, line);
        REQUIRE(line == "-75308643");
        REQUIRE(error_file.eof());
        error_file.close();
    }
    // TODO: without input file

    SECTION("test extra command") {
        fs::path aplusb_cpp_path("extracommand.cpp");
        REQUIRE(fs::exists(aplusb_cpp_path));
        Runner runner(aplusb_cpp_path, config);
        fs::ifstream output_file;
        std::string line;

        REQUIRE(runner.Start(fs::path(), output_path, fs::path(), "2 3") == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "3 5");
        REQUIRE(output_file.eof());
        output_file.close();

        REQUIRE(runner.Start(fs::path(), output_path, fs::path(), "4 5 6 7") == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "5 9");
        REQUIRE(output_file.eof());
        output_file.close();

        REQUIRE(runner.Start(fs::path(), output_path, fs::path(), "1000000000 1000000000") == 0);
        REQUIRE(fs::exists(output_path));
        output_file.open(output_path, std::ios::binary);
        REQUIRE(output_file.is_open());
        std::getline(output_file, line);
        REQUIRE(line == "3 2000000000");
        REQUIRE(output_file.eof());
        output_file.close();
    }
}
