#pragma once

#include <string>

#include "model.hpp"
#include "tokenizer.hpp"

void generate(Model& model, const Tokenizer& tokenizer, std::string prompt,
              int steps);
