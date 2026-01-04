add_executable(muon main.cpp)

target_compile_features(muon PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(muon PRIVATE stdc++exp)
endif ()

target_link_libraries(muon PRIVATE muon_core)
