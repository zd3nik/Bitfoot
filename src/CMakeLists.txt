cmake_minimum_required(VERSION 2.8)
include("${CMAKE_SOURCE_DIR}/init.cmake")
project(bitfoot CXX)

set(OBJ_HDR
    Bitfoot.h
    Defs.h
    Diff.h
    HashTable.h
    Move.h
    Stats.h
)
set(OBJ_SRC
    Bitfoot.cpp
    HashTable.cpp
    Stats.cpp
)

include_directories(. ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/senjo)
add_executable(${PROJECT_NAME} ${OBJ_HDR} ${OBJ_SRC} main.cpp)
target_link_libraries(${PROJECT_NAME} senjo)

add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/epd" "${CMAKE_BINARY_DIR}/epd"
    COMMENT "Copying EPD test files"
)
