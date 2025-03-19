#include "LayoutImagePackage.hpp"
#include "etl/to_string.h"

using namespace configuration;

LayoutImagePackage::LayoutImagePackage(fs::IFileSystem& fs, etl::string_view path)
    : m_package(fs, path.data())
{
}

int LayoutImagePackage::mount()
{
    return m_package.mount();
}

size_t LayoutImagePackage::getNbImages() const
{
    return m_package.getFilesNumber() - IMAGES_START_IDX;
}

bool LayoutImagePackage::getMetadataPath(etl::string<fs::MAX_PATH_SIZE>& retPath) const
{
    return getPath(METADATA_IDX, retPath);
}

bool LayoutImagePackage::getImagePath(size_t idx, etl::string<fs::MAX_PATH_SIZE>& retPath) const
{
    size_t idxInPackage = idx + IMAGES_START_IDX;

    return getPath(idxInPackage, retPath);
}

fs::IFileSystem& LayoutImagePackage::getPackageFileSystem()
{
    return m_package;
}

bool LayoutImagePackage::getPath(size_t idx, etl::string<fs::MAX_PATH_SIZE>& retPath) const
{
    if(idx >= m_package.getFilesNumber())
    {
        return false;
    }

    etl::to_string(idx + 1, retPath); /* The first file in package is "1" */
    return true;
}