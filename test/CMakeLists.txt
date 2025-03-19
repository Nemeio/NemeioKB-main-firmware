cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(Karmeliet_test C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fsanitize=address")

set(RESOURCE_FILES_PATH ${CMAKE_CURRENT_LIST_DIR})
cmake_path(APPEND RESOURCE_FILES_PATH "samples")

add_compile_options(-DAPP_MAJOR_NUMBER_DEF=43 -DAPP_BUILD_NUMBER_DEF=21)
add_compile_options(-DREGION_SWAP_START=0)
add_compile_options(-DREGION_SLOT_1_START=0)
add_compile_options(-DREGION_SLOT_1_END=0)
add_compile_options(--coverage)
add_compile_options(-g)
add_compile_options(-O0)
add_compile_options(-DDEVICE_PROPERTIES_DEBUG_STORAGE)
add_compile_options(-DRESOURCE_FILES_PATH="${RESOURCE_FILES_PATH}")

# Sources files
file(GLOB_RECURSE SOURCES_PLATFORM "../Platform/Linux/*.c" "../Platform/Linux/*.cpp")
file(GLOB_RECURSE SOURCES_MW "../Middlewares/Nemeio/*.c" "../Middlewares/Nemeio/*.cpp")
file(GLOB_RECURSE SOURCES_LFS "../Middlewares/Third_Party/LittleFS/*.c" "../Middlewares/Third_Party/LittleFS/*.cpp")
file(GLOB_RECURSE SOURCES_TOOLS "../Middlewares/Tools/*.c" "../Middlewares/Tools/*.cpp")
file(GLOB_RECURSE SOURCES_TEST "../test/src/*.c" "../test/src/*.cpp")
set(SOURCE_FILES ${SOURCES_PLATFORM} ${SOURCES_MW} ${SOURCES_LFS} ${SOURCES_TOOLS} ${SOURCES_TEST})

# FIXME Reimplement this for new controller/framebuffer management
list(FILTER SOURCE_FILES EXCLUDE REGEX "ScreenController_test.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX "ScreenErrorFilter_test.cpp")

# Includes files
file(GLOB_RECURSE HEADERS_PLATFORM_COMMON "../Platform/Common/*.h" "../Platform/Common/*.hpp")
file(GLOB_RECURSE HEADERS_PLATFORM "../Platform/Linux/*.h" "../Platform/Linux/*.hpp")
file(GLOB_RECURSE HEADERS_INC "../Inc/*.h" "../Inc/*.hpp")
file(GLOB_RECURSE HEADERS_NEMEIO "../Middlewares/Nemeio/*.h" "../Middlewares/Nemeio/*.hpp")
file(GLOB_RECURSE HEADERS_LFS "../Middlewares/Third_Party/LittleFS/*.h" "../Middlewares/Third_Party/LittleFS/*.hpp")
file(GLOB_RECURSE HEADERS_TOOLS "../Middlewares/Tools/*.h" "../Middlewares/Tools/*.hpp")
file(GLOB_RECURSE HEADERS_APP "../App/*.h" "../App/*.hpp")
file(GLOB_RECURSE HEADERS_TEST "../test/*.h" "../test/*.hpp")

set(HEADERS ${HEADERS_PLATFORM_COMMON} ${HEADERS_PLATFORM} ${HEADERS_NEMEIO} ${HEADERS_LFS} ${HEADERS_INC} ${HEADERS_TOOLS} ${HEADERS_APP} ${HEADERS_TEST})

# Add include directories depending on the header files path
FOREACH(header ${HEADERS})
 get_filename_component(dir ${header} DIRECTORY)
 include_directories(${dir})
ENDFOREACH()

# Add only etl include path
include_directories(../Middlewares/Third_Party/etl/include/)

include_directories(../BootloaderSBSFU/Middlewares/ST/STM32_Secure_Engine/Core)
include_directories(../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/Inc)
include_directories(../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SBSFU/SBSFU/App)
include_directories(src/mocks)

# QCBOR library
add_subdirectory(../Middlewares/Third_Party/QCBOR qcborlib)
target_compile_definitions(qcbor PUBLIC USEFULBUF_DISABLE_ALL_FLOAT)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_EXP_AND_MANTISSA)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_TAGS)

add_executable(Karmeliet_test ${SOURCE_FILES})
add_link_options(--coverage)
target_link_libraries(Karmeliet_test gcov)
target_link_libraries(Karmeliet_test gmock)
target_link_libraries(Karmeliet_test gtest)
target_link_libraries(Karmeliet_test pthread)
target_link_libraries(Karmeliet_test qcbor)
