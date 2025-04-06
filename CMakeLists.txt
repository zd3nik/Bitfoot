cmake_minimum_required(VERSION 3.5)

execute_process(
  COMMAND git log "--pretty=format:%H" -n 1
  OUTPUT_VARIABLE GIT_REV)

message("-- GIT_REV ${GIT_REV}")

set(CMAKE_CXX_STANDARD 14)
add_compile_definitions(GIT_REV=${GIT_REV})
if(WIN32)
    add_compile_options(/EHsc)
else()
    add_compile_options(-fPIC -m64 -Wall -Wextra -Wpedantic)
endif()

project(Bitfoot CXX)

add_subdirectory(src)

include_directories(src)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} bitfoot_lib)

add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/epd" "${CMAKE_BINARY_DIR}/epd"
    COMMENT "Copying EPD test files"
)

