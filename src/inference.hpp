#pragma once

#include "model.hpp"
#include "tokenizer.hpp"
#include <string>



void generate(Model& model, const Tokenizer& tokenizer, std::string prompt, int steps);
