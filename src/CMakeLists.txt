project(bitfoot_lib CXX)

add_subdirectory(senjo)

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

include_directories(.)
add_library(${PROJECT_NAME} STATIC ${OBJ_HDR} ${OBJ_SRC})
target_link_libraries(${PROJECT_NAME} senjo)

