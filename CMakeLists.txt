cmake_minimum_required(VERSION 2.6)

project(hoshiMNet CXX)

set(CXX_FLAGS
    -g
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -std=c++11
)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories(${PROJECT_SOURCE_DIR}/hoshiMNet)

add_subdirectory(hoshiMNet)

add_executable(app main.cpp)

target_link_libraries(app hoshiMNetBase)
target_link_libraries(app hoshiMNetNet)
