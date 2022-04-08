#include "catch.hpp"

#include <string>
#include <vector>

#include "filesystem.h"

#include "dataconfigparser.h"

TEST_CASE("test dataconfigparser") {
    fs::path config_path(fs::temp_directory_path() / "test_oigen_data_config_parser.txt");
    fs::ofstream config_file;
    std::vector<std::pair<int, std::string>> config;
    std::vector<std::string> raw_config;
    SECTION("test small config") {
        config = {
            {1, "-n 1 -m 3"},
            {1, "1"},
            {1, "--num=1"},
            {2, "2"},
            {0, "-n 0 -m 3"},
            {5, "--num=5"},
            {5, "5"},
            {6, "-n 6 -m 3"},
            {0, "0"},
            {0, "--num=0"},
        };
        raw_config.clear();
        config_file.open(config_path, std::ios::binary);

        for (auto data : config) {
            if (data.first) config_file << "@" << data.first << std::endl;
            else ++data.first;
            config_file << data.second << std::endl;
            raw_config.insert(raw_config.end(), data.first, data.second);
        }
        raw_config.push_back("");
        config_file.close();
        DataConfigParser data_config_parser(config_path);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
    }
    SECTION("test large config") {
        config = {
            {100, "-n 100 -m 3"},
            {0, "0"},
            {30, "30"},
            {40, "--num=40"},
            {25, "25"},
            {0, "-n 0 -m 3"},
            {35, "--num=35"},
            {125, "125"},
            {70, "-n 70 -m 3"},
            {0, "--num=0"},
        };
        raw_config.clear();
        config_file.open(config_path, std::ios::binary);

        for (auto data : config) {
            if (data.first) config_file << "@" << data.first << std::endl;
            else ++data.first;
            config_file << data.second << std::endl;
            raw_config.insert(raw_config.end(), data.first, data.second);
        }
        raw_config.push_back("");
        config_file.close();
        DataConfigParser data_config_parser(config_path);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
    }
    SECTION("test empty config") {
        config = {};
        raw_config.clear();
        config_file.open(config_path, std::ios::binary);

        raw_config.push_back("");
        config_file.close();

        DataConfigParser data_config_parser(config_path);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
        REQUIRE(data_config_parser.Parse() == true);
        REQUIRE(data_config_parser.GetConfig() == raw_config);
    }
}
