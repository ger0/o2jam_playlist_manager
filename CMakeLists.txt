cmake_minimum_required(VERSION 3.7)
project(o2jam_duplicate_finder)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_path(PICOSHA2_INCLUDE_DIRS "picosha2.h")

file(GLOB SRC_FILES src/*.cpp)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DDEBUG)
endif()


add_executable(o2jdeduper ${SRC_FILES})
target_include_directories(o2jdeduper PRIVATE ${PICOSHA2_INCLUDE_DIRS})
target_link_libraries(o2jdeduper PRIVATE unofficial::sqlite3::sqlite3)
