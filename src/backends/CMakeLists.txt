add_library(muon_backend INTERFACE)


if (MUON_BACKEND STREQUAL "CPU")
    add_subdirectory(cpu)
    target_link_libraries(muon_backend INTERFACE muon_backend_cpu)
elseif (MUON_BACKEND STREQUAL "SYCL")
    add_subdirectory(sycl)
    target_link_libraries(muon_backend INTERFACE muon_backend_sycl)
else ()
    message(FATAL_ERROR "Unsupported Backend. MUON_BACKEND='${MUON_BACKEND}'. Available backends: CPU, SYCL.")
endif ()

