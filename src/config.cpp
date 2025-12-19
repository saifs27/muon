#include "config.hpp"
#include <json.hpp>

enum class ActivationType {
    GELU,
    SILU,
};

void Config::from_file(const std::filesystem::path& file_name) {
    using json = nlohmann::json;



}
