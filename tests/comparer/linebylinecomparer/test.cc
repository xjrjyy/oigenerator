#include "catch.hpp"

#include "linebylinecomparer.h"

void WriteData(const std::string &output, const std::string &answer) {
    static const fs::path output_path(fs::temp_directory_path() / "test_oigen_data_comparer_1.out");
    static const fs::path answer_path(fs::temp_directory_path() / "test_oigen_data_comparer_1.ans");
    fs::ofstream output_file;
    fs::ofstream answer_file;
    output_file.open(output_path, std::ios::binary);
    answer_file.open(answer_path, std::ios::binary);
    output_file << output;
    answer_file << answer;
    output_file.close();
    answer_file.close();
}

TEST_CASE("test LineByLineComparer") {
    LineByLineComparer comparer;
    fs::path output_path(fs::temp_directory_path() / "test_oigen_data_comparer_1.out");
    fs::path answer_path(fs::temp_directory_path() / "test_oigen_data_comparer_1.ans");
    fs::ofstream output_file;
    fs::ofstream answer_file;
    SECTION("test empty file") {
        WriteData("", "");
        REQUIRE(comparer.Compare(output_path, answer_path));
    }
    SECTION("test same file") {
        WriteData("same\n", "same\n");
        REQUIRE(comparer.Compare(output_path, output_path));
        REQUIRE(comparer.Compare(answer_path, answer_path));
    }
    SECTION("test space") {
        WriteData("space \nspace\nsapce \nsapce\n", "space \nspace \nsapce\nsapce\n");
        REQUIRE(comparer.Compare(output_path, answer_path));

        WriteData("space \nspace\nsapce \nsap ce\n", "space \nspace \nsapce\nsapce\n");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("space \nspace\nsapce \nsapce\n", "space \nspace \nsapce\ns apce\n");
        REQUIRE(!comparer.Compare(output_path, answer_path));
    }
    SECTION("test endline") {
        WriteData("\n\n\n\n", "\n\n\n\n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("\n\n\n\n", "\n\n\n\n\n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("\n\n\n\n\n", "\n\n\n\n");
        REQUIRE(comparer.Compare(output_path, answer_path));

        WriteData("endline\nendline", "endline\nendline\n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("endline\nendline\n", "endline\nendline");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("endline\nendline\n", "endline\nendline\n\n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("endline\nendline\n\n", "endline\nendline\n");
        REQUIRE(comparer.Compare(output_path, answer_path));

        WriteData("endline\nendline", "endline\nendline\n\n");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("endline\nendline\n\n", "endline\nendline");
        REQUIRE(!comparer.Compare(output_path, answer_path));
    }
    SECTION("test all") {
        WriteData("space\nendline", "space\nendline \n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("space\nendline\n", "space \nendline \n");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("space \nendline \n", "space\nendline");
        REQUIRE(comparer.Compare(output_path, answer_path));

        WriteData("space\nendline", "space\nendline\n\n");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("sp ace\nendline\n", "space \nendline\n");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("space \nendline\n\n", "space\nendline ");
        REQUIRE(!comparer.Compare(output_path, answer_path));

        WriteData("1 2 \n3 4 \n", "1 2\n3 4");
        REQUIRE(comparer.Compare(output_path, answer_path));
        WriteData("1 2 \n3 4 \n\n", "1 2\n3 4");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("1 2 \n 3 4 \n", "1 2\n3 4");
        REQUIRE(!comparer.Compare(output_path, answer_path));
        WriteData("12 \n3 4 \n\n", "1 2\n3 4");
        REQUIRE(!comparer.Compare(output_path, answer_path));
    }
    // TODO: large file
}
