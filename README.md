## Build instructions for developers

The following instructions are given for a Linux environment. If you are working under Windows, you can use WSL as development environment.

### Initial setup

#### Environment

You can use the docker image witekio/mcu-toolchain-gnu-arm-embedded:24.01 as build environment for the firmware and witekio/mcu-toolchain-test:23.04 for unit tests.

Otherwise, the following tools are required:

- CMake (tested with 3.26)
- arm-none-eabi toolchain v12.2.1
- Python 3
- g++ v12.1.0
- Google Test 1.13.0 library

To flash and debug the project, you will need:

- J-Link Software

Pull the git submodules
```console
git submodule update --init --recursive
```

Install the python modules required to build the project:

```console
cd BootloaderSBSFU/Middlewares/ST/STM32_Secure_Engine/Utilities/KeysAndImages/requirements.txt
pip install -r requirements.txt
```

```console
cd BootloaderSBSFU/Middlewares/ST/STM32_Secure_Engine/Utilities/KeysAndImages/azure_keyvault_signer/requirements.txt
pip install -r requirements.txt
```

#### Local keys

When building on a local machine, the ECDSA public key for update package signature verification must be provided by the user. Copy `Tools/package_signature/public_key/developer_key/ecdsa_pkg_key_public.pem` to `Tools/package_signature/public_key/ecdsa_pkg_key_public.pem`

### Build steps

The project is built using CMake and make. The build scripts will automatically configure the CMake and run the build instructions for bootloader and application.

#### Initializing the build

Run the build initialization script:
```console
./init_build.sh
```

#### Compiling the bootloader

Run the bootloader compilation script:
```console
./build_bootloader.sh
```

#### Compiling the application

The application must be built after each modification of the bootloader.

Run the application compilation script:
```console
./build_app.sh
```
#### Complete build

You can also perform a complete build using the following script:
```console
./buildall.sh
```

Build images are located in `build_release` folder.

### Deploy

To deploy on the target, connect a J-Link to the STM32 and power the target (USB or battery). Run the following command:

```console
cd build_release/Binary/manufacturing
./flash.sh flash
```

This will flash the bootloader and the user application on the target.

### Debug

In VSCode, using the [Cortex Debug Plugin](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug):

 1. Click on the `Run and Debug` button on the right of the window
 2. Click on the `Attach to Application` button on the top left of the window
 3. If RTT traces are activated, `RTT` output will appear in the `TERMINAL` tab of vscode.
 4. You can debug the target.

### Logs and traces

Logs are can be output in either two ways:

- if RTT traces are activated, logs are output to RTT
- otherwise, logs are written to RAM and can be fetched by the Nemeio desktop application

#### Activating RTT traces

Set the RTT_LOG CMake variable to enable RTT traces.

### Units tests

Unit tests are made to be performed on an x86-64 target using GTest/GMock framework. The unit test project is built using CMake and make. Tests are located in `test` folder.

A script is available to build and run the unit tests:
```console
./validate.sh
```