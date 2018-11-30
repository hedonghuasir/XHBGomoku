project(kazmath)

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/kazmath/*.h)
source_group(include\\kazmath FILES ${HEADER_FILES})
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
source_group(src FILES ${SRC_FILES})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(kazmath STATIC ${HEADER_FILES} ${SRC_FILES})
