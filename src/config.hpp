#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

struct Config {
    std::unordered_map<std::string, int> data;
    void from_file(const std::filesystem::path& file_name);
};


