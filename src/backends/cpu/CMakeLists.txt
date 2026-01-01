add_library(muon_backend_cpu math_cpu.cpp)

target_compile_features(muon_backend_cpu PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(muon_backend_cpu PRIVATE stdc++exp)
endif ()

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(muon_backend_cpu PRIVATE OpenMP::OpenMP_CXX)
else ()
    message(FATAL_ERROR "Could not find OpenMP.")
endif ()

target_link_libraries(muon_backend_cpu PUBLIC muon_core)

target_compile_definitions(muon_backend_cpu PUBLIC MUON_BACKEND_CPU=1)


