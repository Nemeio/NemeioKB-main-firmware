#!/bin/bash -e

# Entering correct directory
cd $(dirname "$0")
BASEDIR=$(pwd)

source set_build_env.sh

cd Tools/package_signature
./generate_update_package_key_code.sh .
cd -

cd build_release/Karmeliet

cmake ../../ -DTOOLCHAIN_PATH=../gcc-arm-none-eabi-7-2018-q2-update -DVERSION=${APP_STRING_VERSION_NUMBER} -DAPP_MAJOR_NUMBER=${APP_MAJOR_NUMBER} -DAPP_BUILD_NUMBER=${APP_BUILD_NUMBER} ${ADDITIONAL_CMAKE_FIRMWARE_FLAGS}
if ! make -j$NPROC
then
	exit 1
fi

echo "Generating sfb"
project_dir=$(pwd)
cd ../../BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SECoreBin/SW4STM32
./SECBOOT_ECCDSA_WITH_AES128_CBC_SHA256.sh ${project_dir} "${project_dir}/LDLC-Karmeliet" "${project_dir}/LDLC-Karmeliet.bin" 1 "${APP_BINARY_VERSION_NUMBER}" "${BOOTLOADER_SBSFU_SECURE_DIR}/SBSFU-${SOURCE_KEY}${SECURE_SUFFIX}.elf"
cd ${project_dir}

echo "Copying binaries to the correct location"
cp LDLC-Karmeliet.bin ../Binary/manufacturing/LDLC-Karmeliet${FIRMWARE_BINARY_SUFFIX}-${APP_STRING_VERSION_NUMBER}.bin
cp LDLC-Karmeliet ../Binary/LDLC-Karmeliet${FIRMWARE_BINARY_SUFFIX}-${APP_STRING_VERSION_NUMBER}.elf
cp output.map ../Binary/LDLC-Karmeliet${FIRMWARE_BINARY_SUFFIX}-${APP_STRING_VERSION_NUMBER}.map
mv ../Binary/SBSFU_LDLC-Karmeliet.bin ../Binary/manufacturing/Scratch-Install${FIRMWARE_BINARY_SUFFIX}-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.bin
mv ../Binary/LDLC-Karmeliet.sfb ../Binary/LDLC-Karmeliet${FIRMWARE_BINARY_SUFFIX}-${APP_STRING_VERSION_NUMBER}-${SOURCE_KEY}${SECURE_SUFFIX}.sfb
cp flash.jlink ../Binary/manufacturing/.
cp erase.jlink ../Binary/manufacturing/.
cd ..
cd ..
