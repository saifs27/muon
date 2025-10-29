add_executable(lie main.cpp math_cpu.cpp tensor.cpp tokenizer.cpp)
target_compile_features(lie PRIVATE cxx_std_23)

if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(lie PRIVATE stdc++exp)
endif()

target_include_directories(lie PRIVATE src)
