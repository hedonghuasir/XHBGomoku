project(protobuf-lite)

#TODO armmode for android armeabi

add_definitions("-DGOOGLE_PROTOBUF_NO_THREAD_SAFETY=1")

set(SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/zero_copy_stream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/io/coded_stream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/wire_format_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/repeated_field.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/message_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/generated_message_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/extension_set.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/once.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google/protobuf/stubs/common.cc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_library(protobuf-lite STATIC ${SRC_FILES})
