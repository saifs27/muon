add_executable(muon_app main.cpp)

target_compile_features(muon_app PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(muon_app PRIVATE stdc++exp)
endif ()

target_link_libraries(muon_app PRIVATE muon_core muon_backend)
