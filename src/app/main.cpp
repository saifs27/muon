#include <iostream>
#include <filesystem>
#include "core/utils.hpp"
#include "core/config.hpp"
#include "model/qwen3/qwen3.hpp"
#include "core/safetensors.hpp"
#include "core/tokenizer.hpp"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<model_directory>\n";
        return -1;
    }

    std::error_code ec;
    const std::filesystem::path dir(argv[1]);
    
    if (!std::filesystem::exists(dir, ec)) { 
        std::cerr << "File path: " << dir << " does not exist";
        return -1;
    }

    if (!std::filesystem::is_directory(dir, ec)) {
        std::cerr << "File path " << dir << " is not a directory";
         return -1;
    }
    

    const auto config = get_config(dir / "config.json");
    if (!config.has_value()) {
        std::cerr << "Failed to load config. Error: " << file_err_to_string(config.error());
        return -1;
    }

    const auto tokenizer = Tokenizer::load(config.value(), dir / "tokenizer.json");
    if (!tokenizer.has_value()) { 
        std::cerr << "Failed to load tokenizer. Error: " << file_err_to_string(tokenizer.error());
        return -1;
    }

    const auto sf = SafeTensors::load(dir / "model.safetensors");
    if (!sf.has_value()) {
        std::cerr << "Failed to load safetensors. Error: " << file_err_to_string(sf.error());
        return -1;
    }

    auto model = qwen3::Model::load(sf.value());
    if (!model.has_value()) {
        std::cerr << "Failed to load model_weights.";
        return -1;
    }




    return 0;
}
