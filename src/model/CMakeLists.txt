add_library(model qwen3.cpp)

target_compile_features(model PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(model PRIVATE stdc++exp)
endif ()

target_link_libraries(model PUBLIC muon_core muon_backend)