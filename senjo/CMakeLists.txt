cmake_minimum_required(VERSION 2.8)
include("${CMAKE_SOURCE_DIR}/init.cmake")
project(senjo CXX)

set(OBJ_HDR
    BackgroundCommand.h
    ChessMove.h
    MoveFinder.h
    Platform.h
    Threading.h
    ChessEngine.h
    EngineOption.h
    Output.h
    Square.h
    UCIAdapter.h
)
set(OBJ_SRC
    BackgroundCommand.cpp
    ChessEngine.cpp
    EngineOption.cpp
    MoveFinder.cpp
    Output.cpp
    Threading.cpp
    UCIAdapter.cpp
)

include_directories(.)
add_library(${PROJECT_NAME} STATIC ${OBJ_HDR} ${OBJ_SRC})
if (CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

