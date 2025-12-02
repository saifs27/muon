add_executable(muon main.cpp math_cpu.cpp tensor.cpp tokenizer.cpp)
target_compile_features(muon PRIVATE cxx_std_23)

if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(muon PRIVATE stdc++exp)
endif()

target_link_libraries(muon PRIVATE nlohmann_json)

target_include_directories(muon PRIVATE src)
