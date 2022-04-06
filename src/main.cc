#include <cstdlib>

#include <iostream>
#include <string>
#include <vector>

#include "fmt/core.h"
#include "filesystem.h"
#include "cxxopts.hpp"

#include "config.h"
#include "runner/runner.h"
#include "configparser/configparser.h"

int main(int argc, char *argv[]) {
    cxxopts::Options options("OIGenerator", "OI Testdata Generator");
    
    options.add_options()
        ("r", "Enable recompiling", cxxopts::value<bool>()->default_value("false"))
        ("n,num", "Number of testdata", cxxopts::value<std::size_t>()->default_value("10"))
        ("p,path", "Data path", cxxopts::value<std::string>()->default_value("data"))
        ("g,gen", "Gen path", cxxopts::value<std::string>()->default_value("gen.cpp"))
        ("s,std", "Std path", cxxopts::value<std::string>()->default_value("std.cpp"))
        ("f,file", "Data filename template", cxxopts::value<std::string>()->default_value("data{}"))
        ("i,in", "Input file extension name", cxxopts::value<std::string>()->default_value(".in"))
        ("o,out", "Output file extension name", cxxopts::value<std::string>()->default_value(".out"))
        ("a,ans", "Answer file extension name", cxxopts::value<std::string>()->default_value(".ans"))
        ("compiler-c", "C compiler path", cxxopts::value<std::string>()->default_value(Config::compiler_c_path))
        ("compiler-cpp", "Cpp compiler path", cxxopts::value<std::string>()->default_value(Config::compiler_cpp_path))
        ("command-c", "C compile command", cxxopts::value<std::string>()->default_value(Config::compile_c_command))
        ("command-cpp", "Cpp compile command", cxxopts::value<std::string>()->default_value(Config::compile_cpp_command))
        ("c,config", "Data config path", cxxopts::value<std::string>()->default_value(""))
        ("h,help", "Print usage")
    ;
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    Config::compiler_c_path = result["compiler-c"].as<std::string>();
    Config::compiler_cpp_path = result["compiler-cpp"].as<std::string>();
    Config::compile_c_command = result["command-c"].as<std::string>();
    Config::compile_cpp_command = result["command-cpp"].as<std::string>();
    
    bool recompiling = result["r"].as<bool>();

    std::size_t num_data = result["num"].as<std::size_t>();

    fs::path data_path(result["path"].as<std::string>());
    fs::path gen_path(result["gen"].as<std::string>());
    fs::path std_path(result["std"].as<std::string>());

    std::string data_filename_template = result["file"].as<std::string>();
    std::string input_extension = result["in"].as<std::string>();
    std::string output_extension = result["out"].as<std::string>();
    std::string answer_extension = result["ans"].as<std::string>();

    fs::path data_config_path(result["config"].as<std::string>());
    std::vector<std::string> data_config;

    if (!data_config_path.empty()) {
        ConfigParser config_parser(data_config_path);
        config_parser.Parse();
        data_config = config_parser.GetConfig();
        std::cout << fmt::format("data config size {}", data_config.size()) << std::endl;
    }

    Runner gen_runner(gen_path, recompiling);
    Runner std_runner(std_path, recompiling);
    if (!fs::exists(data_path)) {
        bool result = fs::create_directories(data_path);
        if (!result) {
            std::cerr << fmt::format("Can not create dir {}", data_path.string()) << std::endl;
        }
    }
    for (std::size_t data_id = 1; data_id <= num_data; ++data_id) {
        std::string data_filename = fmt::format(data_filename_template, data_id);
        std::string input_filename = data_filename + input_extension;
        std::string output_filename = data_filename + output_extension;
        std::string answer_filename = data_filename + answer_extension;
        fs::path input_path = data_path / input_filename;
        fs::path output_path = data_path / output_filename;
        fs::path answer_path = data_path / answer_filename;
        if (data_id - 1 < data_config.size()) {
            gen_runner.Start(fs::path(), input_path, fs::path(), data_config[data_id - 1]);
        } else {
            gen_runner.Start(fs::path(), input_path);
        }
        std_runner.Start(input_path, answer_path);
    }
    return 0;
}
