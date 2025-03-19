cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(tools ${TOOLCHAIN_PATH})
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_LD arm-none-eabi-ld)

project(LDLC-Karmeliet C CXX ASM)

include("cmake-tools/tools.cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fno-rtti -flto")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_SYSTEM_NAME FreeRTOS)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

if(TOOLCHAIN_PATH)
    message("Use toolchain ${TOOLCHAIN_PATH}")
else(TOOLCHAIN_PATH)
    message(FATAL_ERROR "No toolchain provided")
endif(TOOLCHAIN_PATH)

if(VERSION)
    message("Use version ${VERSION}")
else()
    message(FATAL_ERROR "No version provided")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

file(GLOB_RECURSE HEADERS_PLATFORM_COMMON "Platform/Common/*.h" "Platform/Common/*.hpp")
file(GLOB_RECURSE HEADERS_PLATFORM "Platform/Target/*.h" "Platform/Target/*.hpp")
file(GLOB_RECURSE HEADERS_INC "Inc/*.h" "Inc/*.hpp")
file(GLOB_RECURSE HEADERS_LIBRARIES "Libraries/*.h" "Libraries/*.hpp")
file(GLOB_RECURSE HEADERS_NEMEIO "Middlewares/Nemeio/*.h" "Middlewares/Nemeio/*.hpp")
file(GLOB_RECURSE HEADERS_TOOLS "Middlewares/Tools/*.h" "Middlewares/Tools/*.hpp")
file(GLOB_RECURSE HEADERS_DRIVERS "Drivers/*.h" "Drivers/*.hpp")
file(GLOB_RECURSE HEADERS_APP "App/*.h" "App/*.hpp")

set(HEADERS ${HEADERS_PLATFORM_COMMON} ${HEADERS_PLATFORM} ${HEADERS_LIBRARIES} ${HEADERS_NEMEIO} ${HEADERS_INC} ${HEADERS_DRIVERS} ${HEADERS_TOOLS} ${HEADERS_APP})

# Add include directories depending on the header files path
FOREACH(header ${HEADERS})
 get_filename_component(dir ${header} DIRECTORY)
 include_directories(${dir})
ENDFOREACH()

# ST Middlewares
# CMSIS_RTOS
include_directories(Middlewares/ST/CMSIS_RTOS/)

# STM32 Cryptographic Library
include_directories(Middlewares/ST/STM32_Crypto/Inc/)

# STM32 USB Device Library
include_directories(Middlewares/ST/STM32_USB_Device_Library/Core/Inc/)

# Third Party Middlewares
# ETL
include_directories(Middlewares/Third_Party/etl/include/)

# LittleFS
include_directories(Middlewares/Third_Party/LittleFS/)
include_directories(Middlewares/Third_Party/LittleFS/bd/)

# TinyDeflate
include_directories(Middlewares/Third_Party/TinyDeflate/)

include_directories(BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SBSFU/SBSFU/App/)
include_directories(BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/Linker_Common/SW4STM32/)
include_directories(BootloaderSBSFU/Middlewares/ST/STM32_Secure_Engine/Core)
include_directories(BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/Inc)

add_compile_options(-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\))
add_compile_options(-DUSE_HAL_DRIVER)
add_compile_options(-DSTM32L496xx)
add_compile_options(-DENABLE_IMAGE_STATE_HANDLING)
add_compile_options(-DAPP_MAJOR_NUMBER_DEF=${APP_MAJOR_NUMBER} -DAPP_BUILD_NUMBER_DEF=${APP_BUILD_NUMBER})
add_compile_options(-O3)
add_compile_options(-g)
add_compile_options(-fdata-sections -ffunction-sections -fno-exceptions)
add_compile_options(-Werror=return-type)
add_compile_options(-DDEFLATE_ALLOCATION_STATIC)
#add_compile_options(-DNO_ITE_UPDATE)

if(INTERNAL_CEM_TESTS)
    add_compile_options(-DCEM_TESTS)
endif()

if(INTERNAL_ROLLBACK_TEST)
    add_compile_options(-DROLLBACK_TEST)
endif()

if(INTERNAL_APP_GOLDEN_IMAGE)
    add_compile_options(-DAPP_GOLDEN_IMAGE)
endif()

if(DEVICE_PROPERTIES_DEBUG_STORAGE)
    add_compile_options(-DDEVICE_PROPERTIES_DEBUG_STORAGE)
endif()

file(GLOB_RECURSE SOURCES_SRC "Src/**")
file(GLOB_RECURSE SOURCES_APP "App/**")
file(GLOB_RECURSE SOURCES_DRIVERS "Drivers/**")
file(GLOB_RECURSE SOURCES_LIBRARIES "Libraries/**")
file(GLOB_RECURSE SOURCES_NEMEIO "Middlewares/Nemeio/**")
file(GLOB_RECURSE SOURCES_TOOLS "Middlewares/Tools/**")
file(GLOB_RECURSE SOURCES_STARTUP "startup/**")
file(GLOB_RECURSE SOURCES_PLATFORM "Platform/Target/**")
file(GLOB_RECURSE SOURCES_PACKAGE_SIGNATURE "Tools/package_signature/**")

# ST Middlewares
file(GLOB SOURCES_ST
    "Middlewares/ST/CMSIS_RTOS/*.c"
    "Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c"
)

# Third Party Middlewares
file(GLOB SOURCES_THIRD_PARTY
    "Middlewares/Third_Party/LittleFS/*.c"
    "Middlewares/Third_Party/LittleFS/bd/*.c"
)

set(SOURCE_FILES ${SOURCES_SRC} ${SOURCES_APP} ${SOURCES_DRIVERS} ${SOURCES_LIBRARIES} ${SOURCES_NEMEIO} ${SOURCES_STARTUP} ${SOURCES_TOOLS} ${SOURCES_PLATFORM} ${SOURCES_PACKAGE_SIGNATURE} ${SOURCES_ST} ${SOURCES_THIRD_PARTY})

if(NOT DEVICE_PROPERTIES_DEBUG_STORAGE)
    list(FILTER SOURCE_FILES EXCLUDE REGEX "DevicePropertiesEnvStorage.cpp$")
endif()

list(FILTER SOURCE_FILES EXCLUDE REGEX "KeyScreen.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "iScreenTest.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "Libraries/FatFS")

# FreeRTOS
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Platform/Target/FreeRTOS CACHE STRING "")

add_subdirectory(Middlewares/Third_Party/FreeRTOS-Kernel)

# QCBOR library
add_subdirectory(Middlewares/Third_Party/QCBOR)
target_compile_definitions(qcbor PUBLIC USEFULBUF_DISABLE_ALL_FLOAT)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_EXP_AND_MANTISSA)
target_compile_definitions(qcbor PUBLIC QCBOR_DISABLE_TAGS)

add_library(libSTM32Cryptographic STATIC IMPORTED)
set_target_properties(libSTM32Cryptographic PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_Crypto/Lib/libSTM32CryptographicV3.0.0_CM4_GCC_FPU.a)

set(PROJECT_LIBRARIES freertos_kernel qcbor libSTM32Cryptographic)
if(RTT_LOG)
    add_compile_options(-DRTT_LOG)
else(RTT_LOG)
    list(FILTER SOURCE_FILES EXCLUDE REGEX "Libraries/SEGGER/*")
endif(RTT_LOG)

add_executable(LDLC-Karmeliet  ${SOURCE_FILES})

set (CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs -Xlinker -L ../.. -L../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/Linker_Common/SW4STM32 -TSTM32L496VGTx_FLASH.ld -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -flto=auto -lm -Wl,--print-memory-usage -Wl,--no-warn-rwx-segments")

target_link_libraries(LDLC-Karmeliet ${PROJECT_LIBRARIES})

add_custom_command(TARGET LDLC-Karmeliet
 POST_BUILD
 COMMAND arm-none-eabi-objcopy -O binary "LDLC-Karmeliet" "LDLC-Karmeliet.bin"
 COMMENT "Invoking: Binary file creation"
)

add_custom_command(TARGET LDLC-Karmeliet
 POST_BUILD
 COMMAND arm-none-eabi-size --format=berkeley "LDLC-Karmeliet"
 COMMENT "Invoking: Cross ARM GNU Print Size"
)
