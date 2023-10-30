project(benchmark)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(ELASTIC_INCLUDE "${PROJECT_SOURCE_DIR}/../include")

message("elastic include: ${ELASTIC_INCLUDE}")

include_directories(${ELASTIC_INCLUDE})


add_executable(benchmark benchmark.cpp)

target_link_libraries(benchmark libprotobuf)

