#!/bin/bash -e

NPROC=`nproc`

# Environment variables can give additional information about build
# We need to set variable to 1 or 0 because of CMake Cache (it uses the old value if not defined)
if [ "x${CEM_TESTS}" = "x1" ]; then
	FIRMWARE_BINARY_SUFFIX="-cem"
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="-DINTERNAL_CEM_TESTS=1"
	echo "------ Building with cem ENABLED ------"
else
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="-DINTERNAL_CEM_TESTS=0"
fi

if [ "x${ROLLBACK_TESTS}" = "x1" ]; then
	FIRMWARE_BINARY_SUFFIX="${FIRMWARE_BINARY_SUFFIX}-rollback-test"
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="${ADDITIONAL_CMAKE_FIRMWARE_FLAGS} -DINTERNAL_ROLLBACK_TEST=1"
	echo "------ Building with rollback tests ENABLED ------"
else
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="${ADDITIONAL_CMAKE_FIRMWARE_FLAGS} -DINTERNAL_ROLLBACK_TEST=0"
fi

if [ "x${ENABLE_SECURE_BOOT}" = "x1" ]; then
	SECURE_SUFFIX=-RDP2
	ADDITIONAL_CMAKE_BOOTLOADER_FLAGS="-DINTERNAL_ENABLE_SECURE_BOOT=1"
	echo "------ Building with secure boot ENABLED ------"
else
	SECURE_SUFFIX=-UNSECURE
	ADDITIONAL_CMAKE_BOOTLOADER_FLAGS="-DINTERNAL_ENABLE_SECURE_BOOT=0"
	echo "------ Building with secure boot DISABLED ------"
fi

if [ "x${GOLDEN_IMAGE}" = "x1" ]; then
	FIRMWARE_BINARY_SUFFIX="${FIRMWARE_BINARY_SUFFIX}-golden-image"
	echo "APP_MAJOR_NUMBER=2047" > version.txt
	echo "APP_BUILD_NUMBER=31" >> version.txt
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="${ADDITIONAL_CMAKE_FIRMWARE_FLAGS} -DINTERNAL_APP_GOLDEN_IMAGE=1"
	echo "------ Building GOLDEN_IMAGE ------"
else
	ADDITIONAL_CMAKE_FIRMWARE_FLAGS="${ADDITIONAL_CMAKE_FIRMWARE_FLAGS} -DINTERNAL_APP_GOLDEN_IMAGE=0"
fi

USING_AZURE_KEYVAULT=false
if ! [ -z "${NEMEIO_AZURE_KEYVAULT_URL}" ]; then
  echo "Using Keyvault URL :" $NEMEIO_AZURE_KEYVAULT_URL
  if ! [ -z "${NEMEIO_AZURE_KEYVAULT_STM32_KEYNAME}" ]; then
    echo "Using Key :" $NEMEIO_AZURE_KEYVAULT_STM32_KEYNAME
    if ! [ -z "${NEMEIO_AZURE_KEYVAULT_STM32_PUBKEY_PEM_PATH}" ]; then
      if [ -f "$NEMEIO_AZURE_KEYVAULT_STM32_PUBKEY_PEM_PATH" ]; then
        echo "Using Public Key :" $NEMEIO_AZURE_KEYVAULT_STM32_PUBKEY_PEM_PATH
        USING_AZURE_KEYVAULT=true
      else
        echo "Public key file" $NEMEIO_AZURE_KEYVAULT_STM32_PUBKEY_PEM_PATH "doesn't exist!"
      fi
    else
      echo "Public Key is not set"
    fi
  else
    echo "Key name is not set"
  fi
else
  echo "Keyvault URL is not set"
  echo "AzureKeyVaultUrl variable is" $AZUREKEYVAULTURL
  echo "AzureKeyVaultNrfKeyName variable is" $AZUREKEYVAULTNRFKEYNAME
fi

export USING_AZURE_KEYVAULT

if [ "$USING_AZURE_KEYVAULT" = true ] ; then
	SOURCE_KEY="azure"
else
	SOURCE_KEY="local"
fi

ADDITIONAL_CMAKE_BOOTLOADER_FLAGS="${ADDITIONAL_CMAKE_BOOTLOADER_FLAGS} -DSOURCE_KEY=${SOURCE_KEY}"

BOOTLOADER_SBSFU_SECURE_DIR=${BASEDIR}/BootloaderSBSFU/Projects/STM32L496-Karmeliet/Applications/2_Images/2_Images_SBSFU/SW4STM32/STM32L496-Karmeliet_2_Images_SBSFU/Debug/

echo "Source version number"
source set_version.sh