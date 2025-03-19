#include "FileSystemPartitions.hpp"
#include "LinuxFileSystem.hpp"
#include "QSPIMapping.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

const char* QSPI_FILE_NAME = "qspi.bin";

struct QSPIPartitionInfo
{
    const char* name;
    const char* source_file;
    size_t offset;
    size_t size;

    QSPIPartitionInfo(const char* name, const char* source_file, size_t offset, size_t size)
        : name(name)
        , source_file(source_file)
        , offset(offset)
        , size(size){};
};

int main(int argc, char** argv)
{
    int ret = 0;
    std::remove(QSPI_FILE_NAME);

    if(argc != 5)
    {
        std::cout << "Missing files path as input: <Environment A> <Environment B> "
                     "<File system> "
                     "<Update package>"
                  << std::endl;
        return 1;
    }

    std::vector<struct QSPIPartitionInfo> parts;

    parts.push_back(
        QSPIPartitionInfo("Environment A", argv[1], PARTITION_INFORMATION(FIRMWARE_ENV_BANK_A)));
    parts.push_back(
        QSPIPartitionInfo("Environment B", argv[2], PARTITION_INFORMATION(FIRMWARE_ENV_BANK_B)));
    parts.push_back(QSPIPartitionInfo("File system", argv[3], PARTITION_INFORMATION(MAIN)));
    parts.push_back(
        QSPIPartitionInfo("Update package", argv[4], PARTITION_INFORMATION(FIRMWARE_UPDATE_BANK_A)));

    /* Debug traces */
    std::cout << "Partition name"
              << "\t\t"
              << "Source file"
              << "\t\t"
              << "Partition offset"
              << "\t"
              << "Partition size" << std::endl;
    std::cout << std::endl;
    for(auto part: parts)
    {
        std::cout << part.name << "\t\t" << part.source_file << "\t\t" << part.offset << "\t\t\t"
                  << part.size << std::endl;
    }

    static uint8_t QSPI[QSPI_FLASH_SIZE];

    /* By default flash is erased with 0xFF */
    std::fill(std::begin(QSPI), std::end(QSPI), 0xff);

    for(auto part: parts)
    {
        std::cout << std::endl << "Filling QSPI with " << part.source_file << ": ";

        std::ifstream binary(part.source_file, std::fstream::in | std::fstream::binary);

        binary.seekg(0, binary.end);
        int length = binary.tellg();
        binary.seekg(0, binary.beg);

        if(!binary.is_open())
        {
            std::cout << "Invalid file";
            ret = 1;
            break;
        }

        if(length > part.size)
        {
            std::cout << "Invalid size";
            ret = 1;
            break;
        }

        binary.read(reinterpret_cast<char*>(QSPI + part.offset), part.size);

        std::cout << "OK";
    }

    std::cout << std::endl;

    if(!ret)
    {
        /* Generate output QSPI file */
        std::fstream qspi;
        qspi.open(QSPI_FILE_NAME, std::fstream::out | std::fstream::binary | std::fstream::trunc);
        qspi.write(reinterpret_cast<char*>(QSPI), QSPI_FLASH_SIZE);
    }

    return ret;
}
