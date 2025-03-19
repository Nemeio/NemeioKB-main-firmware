#!/bin/bash -e

# Entering correct directory
cd $(dirname "$0")
BASEDIR=$(pwd)

source set_build_env.sh

cd build_release

# Generate key in assembly for bootloader
cd ../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/SW4STM32
./prebuild.sh .
cd -

cd SECoreBin
cmake ../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/ -DTOOLCHAIN_PATH=../gcc-arm-none-eabi-7-2018-q2-update -DVERSION=0000 ${ADDITIONAL_CMAKE_BOOTLOADER_FLAGS}
if ! make -j$NPROC
then
	exit 1
fi

mkdir -p ../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/SW4STM32/STM32L496-Karmeliet_2_Images_SECoreBin/Debug/
cp LDLC-Karmeliet-SECoreBin-${SOURCE_KEY}${SECURE_SUFFIX}.bin ../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/SW4STM32/STM32L496-Karmeliet_2_Images_SECoreBin/Debug/SECoreBin-${SOURCE_KEY}${SECURE_SUFFIX}.bin
cp LDLC-Karmeliet-SECoreBin-${SOURCE_KEY}${SECURE_SUFFIX} ../Binary/LDLC-Karmeliet-SECoreBin-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.elf
cp output.map ../Binary/LDLC-Karmeliet-SECoreBin-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.map
cd ..

cd SBSFU
cmake ../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SBSFU/ -DTOOLCHAIN_PATH=../gcc-arm-none-eabi-7-2018-q2-update -DVERSION=0000 ${ADDITIONAL_CMAKE_BOOTLOADER_FLAGS}
if ! make -j$NPROC
then
	exit 1
fi

cp LDLC-Karmeliet-SBSFU-${SOURCE_KEY}${SECURE_SUFFIX}.bin ../Binary/manufacturing/LDLC-Karmeliet-SBSFU-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.bin
mkdir -p ${BOOTLOADER_SBSFU_SECURE_DIR}
cp LDLC-Karmeliet-SBSFU-${SOURCE_KEY}${SECURE_SUFFIX} ${BOOTLOADER_SBSFU_SECURE_DIR}/SBSFU-${SOURCE_KEY}${SECURE_SUFFIX}.elf
cp LDLC-Karmeliet-SBSFU-${SOURCE_KEY}${SECURE_SUFFIX} ../Binary/LDLC-Karmeliet-SBSFU-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.elf
cp output.map ../Binary/LDLC-Karmeliet-SBSFU-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.map

cp se_interface_app.ld ../../se_interface_app.ld
cd ..