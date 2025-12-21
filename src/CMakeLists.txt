add_executable(muon main.cpp math_cpu.cpp tensor.cpp tokenizer.cpp safetensors.cpp)
target_compile_features(muon PRIVATE cxx_std_23)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(muon PRIVATE stdc++exp)
endif()


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(muon PRIVATE OpenMP::OpenMP_CXX)
endif()


add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${PROJECT_SOURCE_DIR}/vendor/nlohmann)
target_link_libraries(muon PRIVATE nlohmann_json)

target_include_directories(muon PRIVATE src)
