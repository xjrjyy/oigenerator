#include "dataconfigparser.h"

#include <iostream>

#include "fmt/core.h"

DataConfigParser::DataConfigParser(const fs::path &path)
    : path_(path) {}

bool DataConfigParser::Parse() {
    config_.clear();
    if (!fs::exists(path_)) return false;
    fs::ifstream config_file(path_, std::ios::binary);
    if (!config_file.is_open()) return false;
    while (!config_file.eof()) {
        std::string line;
        std::getline(config_file, line);
        int repeat = 1;
        while (line.size() > 0 && line[0] == '@') {
            line = line.substr(1);
            if (line.find(' ') != line.npos) line.erase(line.find(' '));
            repeat *= std::stoi(line);
            std::getline(config_file, line);
            if (config_file.eof()) break;
        }
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
        // std::cout << repeat << ": " << line << std::endl;
        config_.insert(config_.end(), repeat, line);
    }
    return true;
}

std::vector<std::string> DataConfigParser::GetConfig() const {
    return config_;
}