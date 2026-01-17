add_library(muon_core 
        config.cpp
        memory_map.cpp
        safetensors.cpp
        sample.cpp
        tokenizer.cpp
        utils.cpp)

target_compile_features(muon_core PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(muon_core PRIVATE stdc++exp)
endif ()


add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${PROJECT_SOURCE_DIR}/vendor/nlohmann)

target_link_libraries(muon_core PUBLIC nlohmann_json)
target_include_directories(muon_core PUBLIC ${PROJECT_SOURCE_DIR}/src)
