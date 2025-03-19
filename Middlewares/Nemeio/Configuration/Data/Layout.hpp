#pragma once
#include "ScreenDefs.hpp"
#include "Uuid.hpp"
#include "etl/functional.h"
#include "etl/string.h"
#include "etl/optional.h"
#include "LayoutImagePackage.hpp"

namespace configuration
{
struct LayoutMetadata
{
    ScreenDefs::BackgroundColor background;
    ScreenDefs::PixelFormat format;

    bool operator==(const LayoutMetadata& layout) const
    {
        return background == layout.background && format == layout.format;
    }
};

class Layout
{
public:
    Layout(uuid::UuidView hash,
           const LayoutMetadata& metadata,
           size_t nbImages,
           fs::IFileSystem& packageFs,
           etl::string_view packagePath);
    virtual ~Layout() = default;

    LayoutImagePackage getPackage() const;
    const LayoutMetadata& getMetadata() const { return m_metadata; }
    size_t getNbImages() const { return m_nbImages; }

    bool operator==(const Layout& layout) const;

private:
    uuid::Uuid m_hash;
    LayoutMetadata m_metadata;
    size_t m_nbImages;
    etl::reference_wrapper<fs::IFileSystem> m_packageFs;
    etl::string<fs::MAX_PATH_SIZE> m_packagePath;
};
} // namespace configuration