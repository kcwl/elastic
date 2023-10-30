project(benchmark)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(ELASTIC_INCLUDE "${PROJECT_SOURCE_DIR}/../include")

message("elastic include: ${ELASTIC_INCLUDE}")

include_directories(${ELASTIC_INCLUDE})

set(SRC_LIST 
benchmark.cpp
data.pb.cc
)


add_executable(benchmark ${SRC_LIST})

target_link_libraries(benchmark libprotobuf)

