cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(PrepareImage C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_compile_options(-g)
add_compile_options(-O0)

set(PROJECT_SRC_ROOT ../../../)

# Sources files
file(GLOB_RECURSE SOURCES_PLATFORM "${PROJECT_SRC_ROOT}/Platform/Linux/*.c" "${PROJECT_SRC_ROOT}/Platform/Linux/*.cpp")

set(SOURCE_FILES
    PrepareImage.cpp
    ${PROJECT_SRC_ROOT}/Middlewares/Nemeio/System/Lock/AbstractMutex.cpp
    ${PROJECT_SRC_ROOT}/Middlewares/Nemeio/System/Lock/AutoLock.cpp
    ${PROJECT_SRC_ROOT}/Middlewares/Nemeio/Environment/FirmwareEnvironmentRedundant.cpp
    ${PROJECT_SRC_ROOT}/Middlewares/Nemeio/Environment/FirmwareEnvironment.cpp
    ${PROJECT_SRC_ROOT}/Middlewares/Nemeio/FileSystem/File.cpp
    ${SOURCES_PLATFORM}
    ${SOURCES_FILESYSTEM}
    ${SOURCES_LFS}
)

# Includes files
file(GLOB_RECURSE HEADERS_PLATFORM_COMMON "${PROJECT_SRC_ROOT}/Platform/Common/*.h" "${PROJECT_SRC_ROOT}/Platform/Common/*.hpp")
file(GLOB_RECURSE HEADERS_PLATFORM "${PROJECT_SRC_ROOT}/Platform/Linux/*.h" "${PROJECT_SRC_ROOT}/Platform/Linux/*.hpp")
file(GLOB_RECURSE HEADERS_NEMEIO "${PROJECT_SRC_ROOT}/Middlewares/Nemeio/*.h" "${PROJECT_SRC_ROOT}/Middlewares/Nemeio/*.hpp")
file(GLOB_RECURSE HEADERS_TOOLS "${PROJECT_SRC_ROOT}/Middlewares/Tools/*.h" "${PROJECT_SRC_ROOT}/Middlewares/Tools/*.hpp")
file(GLOB_RECURSE HEADERS_LFS "${PROJECT_SRC_ROOT}/Middlewares/Third_Party/LittleFS/*.h" "${PROJECT_SRC_ROOT}/Middlewares/Third_Party/LittleFS/*.hpp")

set(HEADERS ${HEADERS_PLATFORM_COMMON} ${HEADERS_PLATFORM} ${HEADERS_NEMEIO} ${HEADERS_LFS} ${HEADERS_INC} ${HEADERS_TOOLS} ${HEADERS_APP} ${HEADERS_TEST})

# Add include directories depending on the header files path
FOREACH(header ${HEADERS})
 get_filename_component(dir ${header} DIRECTORY)
 include_directories(${dir})
ENDFOREACH()

# Add only etl include path
include_directories(${PROJECT_SRC_ROOT}/Middlewares/Third_Party/etl/include/)
include_directories(${PROJECT_SRC_ROOT}/Libraries/Assembly/AssemblyFileSystem)
include_directories(${PROJECT_SRC_ROOT}/Inc/)

add_executable(PrepareImage ${SOURCE_FILES})
