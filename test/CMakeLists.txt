cmake_minimum_required(VERSION 3.2)

project(elastic_test)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(Boost_INCLUDE_DIR "/usr/include")



if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(Boost_LIBRARY_DIR /lib64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(Boost_LIBRARY_DIR /lib)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

add_executable(elastic_test Test.cpp)


if(Boost_FOUND)
target_link_libraries(
	elastic_test Boost::unit_test_framework
)

target_compile_definitions(
	elastic_test
	PRIVATE
  	BOOST_TEST_DYN_LINK
  )
else()
message(FATAL_ERROR "boost is not found")
endif()


endif()


enable_testing()
 
add_test(
	NAME elatic_unit_test
	COMMAND $<TARGET_FILE:elastic_test>
	)