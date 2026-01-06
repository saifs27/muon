#include <iostream>
#include <filesystem>

#include "../core/config.hpp"
#include "../core/model.hpp"
#include "../core/safetensors.hpp"
#include "../core/tokenizer.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    std::error_code ec;
    std::filesystem::path dir(argv[1]);
    
    if (!std::filesystem::exists(dir, ec)) { return -1;}
    if (!std::filesystem::is_directory(dir, ec)) { return -1;}
    

    const auto config = get_config(dir / "config.json");
    if (!config.has_value()) { return -3;}

    const auto tokenizer = Tokenizer::load(config.value(), dir / "tokenizer.json");
    if (!tokenizer.has_value()) { return -4;}

    const auto sf = SafeTensors::load(dir / "model.safetensors");
    if (!sf.has_value()) {return -5;}



    return 0;
}
