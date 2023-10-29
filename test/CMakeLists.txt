project(elastic_test)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)


add_executable(elastic_test Test.cpp)

target_link_libraries(elastic_test boost)

enable_testing()
 
add_test(
	NAME elatic_unit_test
	COMMAND $<TARGET_FILE:elastic_test>
	)