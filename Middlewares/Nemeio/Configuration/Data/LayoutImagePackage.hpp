#pragma once
#include "ScreenDefs.hpp"
#include "Uuid.hpp"
#include "ConfigurationCommon.hpp"
#include "PackageFileSystem.hpp"
#include "etl/string.h"

namespace configuration
{
class LayoutImagePackage
{
public:
    LayoutImagePackage(fs::IFileSystem& fs, etl::string_view path);
    virtual ~LayoutImagePackage() = default;

    int mount();
    size_t getNbImages() const;
    bool getMetadataPath(etl::string<fs::MAX_PATH_SIZE>& retPath) const;
    bool getImagePath(size_t idx, etl::string<fs::MAX_PATH_SIZE>& retPath) const;
    fs::IFileSystem& getPackageFileSystem();

private:
    static constexpr size_t METADATA_IDX = 0;
    static constexpr size_t IMAGES_START_IDX = METADATA_IDX + 1;
    PackageFileSystem m_package;

    bool getPath(size_t idx, etl::string<fs::MAX_PATH_SIZE>& retPath) const;
};
} // namespace configuration