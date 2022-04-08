#include "linebylinecomparer.h"

#include <string>

bool LineByLineComparer::Compare(
    const fs::path &output_path,
    const fs::path &answer_path
) const {
    if (fs::equivalent(output_path, answer_path)) return true;
    fs::ifstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        // TODO: log: Cannot open user's output file
        return false;
    }
    fs::ifstream answer_file(answer_path, std::ios::binary);
    if (!answer_file.is_open()) {
        // TODO: log: Cannot open standard output file
        return false;
    }
    std::string output_line, answer_line;
    while (!output_file.eof() && !answer_file.eof()) {
        std::getline(output_file, output_line);
        Trim(output_line);
        std::getline(answer_file, answer_line);
        Trim(answer_line);
        if (output_line != answer_line) return false;
    }
    if (!output_file.eof()) {
        std::getline(output_file, output_line);
        Trim(output_line);
        if (!output_line.empty() || !output_file.eof()) return false;
    }
    if (!answer_file.eof()) {
        std::getline(answer_file, answer_line);
        Trim(answer_line);
        if (!answer_line.empty() || !answer_file.eof()) return false;
    }
    return true;
}

void LineByLineComparer::Trim(std::string &str) {
    while (!str.empty() && IsSpace(str.back())) str.pop_back();
}

bool LineByLineComparer::IsSpace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}