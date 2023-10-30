find_package(Protobuf REQUIRED)

add_library(libprotobuf INTERFACE)

message("protobuf libraries: ${Protobuf_LIBRARIES}")

target_include_directories(libprotobuf INTERFACE ${Protobuf_INCLUDE_DIRS})

target_link_libraries(libprotobuf INTERFACE ${Protobuf_LIBRARIES})