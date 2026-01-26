cmake_minimum_required(VERSION 3.28)

project(muon VERSION 1.0
                      DESCRIPTION "An LLM inference engine"
                      LANGUAGES CXX)


set(MUON_BACKEND "CPU" CACHE STRING "")
set_property(CACHE MUON_BACKEND PROPERTY STRINGS CPU SYCL)



# Helper functions
function(target_enable_all_warnings TARGET_NAME)
    if (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()
 
function(target_enable_asan TARGET_NAME)
    if (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:/fsanitize=address;/Zi>")
        target_link_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:/fsanitize=address>")

    else()
        target_compile_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address;-fno-omit-frame-pointer>")
        target_link_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")
    endif()
endfunction()

add_subdirectory(src)



