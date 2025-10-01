cmake_minimum_required(VERSION 3.7)
project(o2jam_duplicate_finder)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/submodules)

file(GLOB SRC_FILES src/*.cpp)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DDEBUG)
endif()

add_executable(o2jdeduper ${SRC_FILES})
target_link_libraries(o2jdeduper PRIVATE SQLite::SQLite3)
