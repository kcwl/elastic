message("find boost")

find_package(Boost QUIET NO_MODULE)

if(Boost_FOUND)
message("Boost $(Boost_FIND_VERSION)")

if(Boost_FIND_COMPONENTS)
message("Boost find components:")
message(" $(Boost_FIND_COMPONENTS)")
else()
message(FATAL_ERROR "boost is not found")
endif()