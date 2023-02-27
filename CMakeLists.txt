project(YPCAlgoExample)
cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(YPC CONFIG REQUIRED)

include_directories(${SGX_INCLUDE_DIR})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(${YPC_INCLUDE_DIR})
link_directories(${SGX_LIBRARY_DIR})
link_directories(${YPC_LIB_DIR}/)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)

message(STATUS "lib: " ${YPC_LIB_DIR})
message(STATUS "inc: " ${YPC_INCLUDE_DIR})

add_subdirectory(example)
add_subdirectory(integrate)
