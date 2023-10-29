cmake_minimum_required(VERSION 3.2)

project(elastic)

list(APPEND CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/cmake"
)

add_subdirectory(depends)

add_subdirectory(test)

