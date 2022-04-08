#include <cstdlib>

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "fmt/core.h"
#include "filesystem.h"
#include "cxxopts.hpp"

#include "config.h"
#include "runner/runner.h"
#include "configparser/configparser.h"

#include "comparer.h"
#include "comparer/linebylinecomparer/linebylinecomparer.h"

Config ParseConfig(cxxopts::ParseResult result) {
    std::map<std::string, std::string> config_map;
    config_map["compiler_c_path"] = result["compiler-c"].as<std::string>();
    config_map["compiler_cpp_path"] = result["compiler-cpp"].as<std::string>();
    config_map["compile_c_command"] = result["command-c"].as<std::string>();
    config_map["compile_cpp_command"] = result["command-cpp"].as<std::string>();
    config_map["recompiling"] = (result["recompile"].as<bool>() ? "true" : "false");
    return Config(config_map);
}

int Generate(cxxopts::ParseResult result) {
    Config config(ParseConfig(result));

    std::size_t num_data = result["num"].as<std::size_t>();

    fs::path data_path(result["path"].as<std::string>());
    fs::path gen_path(result["gen"].as<std::string>());
    fs::path std_path(result["std"].as<std::string>());
    fs::path usr_path(result["usr"].as<std::string>());

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

    Runner gen_runner(gen_path, config);
    Runner std_runner(std_path, config);
    Runner usr_runner(usr_path, config);
    bool comparing = result["compare"].as<bool>();
    Comparer *comparer = nullptr;
    // TODO: choose comparer
    if (comparing) comparer = new LineByLineComparer();

    bool show_data_id = result["show-id"].as<bool>();

    if (!fs::exists(data_path)) {
        bool result = fs::create_directories(data_path);
        if (!result) {
            std::cerr << fmt::format("Can not create dir {}", data_path.string()) << std::endl;
            return 1;
        }
    }
    if (!data_config.empty() && data_config.size() < num_data) {
        std::cout << fmt::format("Warning: "
            "You want to generate {} sets of data, "
            "but only {} sets are configured",
            num_data,
            data_config.size()
        ) << std::endl;
    }
    for (std::size_t data_id = 1; data_id <= num_data; ++data_id) {
        if (show_data_id) std::cout << fmt::format("Data #{}", data_id) << std::endl;
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
        if (fs::exists(usr_path)) {
            usr_runner.Start(input_path, output_path);
            if (comparing) {
                if (!comparer->Compare(output_path, answer_path)) {
                    std::cout << fmt::format(
                        "There is a difference between output({}) and answer({}).",
                        output_path.string(),
                        answer_path.string()
                    ) << std::endl;
                    std::cout << "Do you want to continue? (y/n)" << std::endl;
                    char choice = '\0';
                    while (true) {
                        std::cin >> choice;
                        choice = tolower(choice);
                        if (choice == 'y' || choice == 'n') break;
                    }
                    if (choice == 'n') return 1;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("OIGenerator", "OI Testdata Generator");
    Config default_config;
    
    options.add_options()
        ("r,recompile", "Enable recompiling", cxxopts::value<bool>()->default_value(default_config.GetRecompiling() ? "true" : "false"))
        ("n,num", "Number of testdata", cxxopts::value<std::size_t>()->default_value("10"))
        ("p,path", "Data path", cxxopts::value<std::string>()->default_value("data"))
        ("g,gen", "Gen path", cxxopts::value<std::string>()->default_value("gen.cpp"))
        ("s,std", "Std path", cxxopts::value<std::string>()->default_value("std.cpp"))
        ("u,usr", "User code path", cxxopts::value<std::string>()->default_value(""))
        ("f,file", "Data filename template", cxxopts::value<std::string>()->default_value("data{}"))
        ("i,in", "Input file extension name", cxxopts::value<std::string>()->default_value(".in"))
        ("o,out", "Output file extension name", cxxopts::value<std::string>()->default_value(".out"))
        ("a,ans", "Answer file extension name", cxxopts::value<std::string>()->default_value(".ans"))
        ("compiler-c", "C compiler path", cxxopts::value<std::string>()->default_value(default_config.GetCompilerCPath()))
        ("compiler-cpp", "Cpp compiler path", cxxopts::value<std::string>()->default_value(default_config.GetCompilerCppPath()))
        ("command-c", "C compile command", cxxopts::value<std::string>()->default_value(default_config.GetCompileCCommand()))
        ("command-cpp", "Cpp compile command", cxxopts::value<std::string>()->default_value(default_config.GetCompileCppCommand()))
        ("c,config", "Data config path", cxxopts::value<std::string>()->default_value(""))
        ("compare", "Enable comparing output with answer", cxxopts::value<bool>()->default_value("true"))
        ("show-id", "Enable showing data id", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
    ;
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    return Generate(result);
}
