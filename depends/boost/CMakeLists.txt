find_package(Boost 1.75 REQUIRED unit_test_framework)

add_library(boost INTERFACE)

message("Boost library path: ${Boost_LIBRARIES}")

target_include_directories(boost
  INTERFACE
    ${Boost_INCLUDE_DIRS})


target_link_libraries(boost
  INTERFACE
    ${Boost_LIBRARIES})

target_compile_definitions(
	boost
	INTERFACE
  	-DBOOST_TEST_DYN_LINK
  )