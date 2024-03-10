project(elastic_test)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(${ELASTIC_INCLUDE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(INCLUDE_FILES 
	flex_buffer.h
	inherit.h
	type.h
)

set(SRCS
	Test.cpp
)

add_executable(elastic_test ${INCLUDE_FILES} ${SRCS})

target_include_directories(elastic_test PUBLIC ${GTEST_INCLUDE})

target_link_directories(elastic_test PUBLIC ${GTEST_LIB})

target_link_libraries(elastic_test ${LIBS})
 
add_test(
	NAME elatic_unit_test
	COMMAND $<TARGET_FILE:elastic_test>
	)

if(CMAKE_COVARAGE)
setup_target_for_coverage_gcovr_xml(
	${PROJECT_NAME}_cover NAME coverage
)