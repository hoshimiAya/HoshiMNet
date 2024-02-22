cmake_minimum_required(VERSION 2.6)

project(hoshiMNet CXX)

set(CMAKE_CXX_FLAGS
    -std=c++11
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories(${PROJECT_SOURCE_DIR}/hoshiMNet)

add_subdirectory(hoshiMNet)

add_executable(app main.cpp)

target_link_libraries(app hoshiMNet)
