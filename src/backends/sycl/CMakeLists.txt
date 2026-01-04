add_library(muon_backend_sycl softmax.cpp rmsnorm.cpp)

target_compile_features(muon_backend_sycl PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(muon_backend_sycl PRIVATE stdc++exp)
endif ()

target_link_libraries(muon_backend_sycl PUBLIC muon_core)

target_compile_definitions(muon_backend_sycl PUBLIC MUON_BACKEND_SYCL=1)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
    target_compile_options(muon_backend_sycl PRIVATE -fsycl)
    target_link_options(muon_backend_sycl PRIVATE -fsycl)
endif ()