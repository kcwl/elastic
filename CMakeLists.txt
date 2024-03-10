cmake_minimum_required(VERSION 3.2)

if(WIN32)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(GTEST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/build/googletest/googletest/include)
set(GTEST_LIB ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

if(WIN32)
set(LIBS gtest.lib gtest_main.lib)
else()
set(LIBS libgtest.a libgtest_main.a)
endif()

set(ELASTIC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

project(elastic)

enable_testing()

add_subdirectory(test)

#add_subdirectory(benchmark)