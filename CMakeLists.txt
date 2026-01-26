cmake_minimum_required(VERSION 3.28)

project(muon VERSION 1.0
                      DESCRIPTION "An LLM inference engine"
                      LANGUAGES CXX)


set(MUON_BACKEND "CPU" CACHE STRING "")
set_property(CACHE MUON_BACKEND PROPERTY STRINGS CPU SYCL)
include(CheckCXXCompilerFlag)


# Helper functions
function(target_enable_all_warnings TARGET_NAME)
    if (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()


function(target_enable_asan TARGET_NAME)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        return()
    endif()

    if (MSVC)
        set(ASAN_COMPILE_FLAG "/fsanitize=address")
        set(ASAN_DEBUG_FLAG "/Zi")
    else()
        set(ASAN_COMPILE_FLAG "-fsanitize=address")
        set(ASAN_DEBUG_FLAG "-fno-omit-frame-pointer")
    endif()

    set(CMAKE_REQUIRED_LINK_OPTIONS "${ASAN_COMPILE_FLAG}")
    check_cxx_compiler_flag("${ASAN_COMPILE_FLAG}" SUPPORTS_ASAN)
    unset(CMAKE_REQUIRED_LINK_OPTIONS)
    
    if (NOT SUPPORTS_ASAN)
        return()
    endif()

    target_compile_options(${TARGET_NAME} PRIVATE ${ASAN_COMPILE_FLAG} ${ASAN_DEBUG_FLAG})
    target_link_options(${TARGET_NAME} PRIVATE ${ASAN_COMPILE_FLAG})
endfunction()

function(target_enable_ubsan TARGET_NAME)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        return()
    endif()
    check_cxx_compiler_flag(-fsanitize=undefined SUPPORTS_UBSAN)

    if (NOT SUPPORTS_UBSAN)
        return()      
    endif()

    target_compile_options(${TARGET_NAME} PUBLIC -fsanitize=undefined)
    target_link_options(${TARGET_NAME} PUBLIC -fsanitize=undefined)
endfunction()

function(target_enable_all TARGET_NAME)
    target_enable_all_warnings(${TARGET_NAME})
    target_enable_asan(${TARGET_NAME})
    target_enable_ubsan(${TARGET_NAME})
    set_target_properties(${TARGET_NAME} PROPERTIES UNITY_BUILD ON)
endfunction()

add_subdirectory(src)



