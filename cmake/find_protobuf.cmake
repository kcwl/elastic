message("find protobuf")

find_package(protobuf QUIET)

if(protobuf_Found)
message("protobuf ${protobu_FIND_VERSION}")
else
message(FATAL_ERROR "protobuf is not find")
endif()