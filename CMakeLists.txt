cmake_minimum_required(VERSION 3.31)

project(muon VERSION 1.0
                      DESCRIPTION "An LLM inference engine"
                      LANGUAGES CXX)

set(MUON_BACKEND "CPU" CACHE STRING "")
set_property(CACHE MUON_BACKEND PROPERTY STRINGS CPU SYCL)


add_subdirectory(src)
