cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(LfsGenerator C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

include_directories(../../../Middlewares/Third_Party/LittleFS)

file(GLOB_RECURSE SOURCES_SRC "src/*.c")
file(GLOB_RECURSE SOURCES_LFS "../../../Middlewares/Third_Party/LittleFS/*.c")
set(SOURCE_FILES ${SOURCES_SRC} ${SOURCES_LFS})

add_compile_options(-Werror=return-type)

add_executable(LfsGenerator ${SOURCE_FILES})
