#include "FirmwareEnvironmentRedundant.hpp"
#include "CRC32.hpp"
#include "LinuxFileSystem.hpp"
#include <cstdint>
#include <iostream>

int main()
{
    LinuxFileSystem fileSystem;
    CRC32 crc32;
    FirmwareEnvironment env;
    FirmwareEnvironmentRedundant firmwareEnvRedundant(crc32,
                                                      fileSystem,
                                                      "./envA.bin",
                                                      fileSystem,
                                                      "./envB.bin");

    bool ret = firmwareEnvRedundant.reset() && firmwareEnvRedundant.getActiveEnvironment(env);
    if(ret)
    {
        env.setNextFirmwareUpdateBank(FirmwareUpdateBank::BANK_B);
        ret = firmwareEnvRedundant.writeEnvironment(env);
    }

    return ret ? 0 : 1;
}