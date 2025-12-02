cmake_minimum_required(VERSION 3.31)

project(llm-inference VERSION 1.0
                      DESCRIPTION "An LLM inference engine"
                      LANGUAGES CXX)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${PROJECT_SOURCE_DIR}/vendor/nlohmann)

add_subdirectory(src)
