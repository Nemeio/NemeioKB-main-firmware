#include "Layout.hpp"

using namespace configuration;

Layout::Layout(uuid::UuidView hash,
               const LayoutMetadata& metadata,
               size_t nbImages,
               fs::IFileSystem& packageFs,
               etl::string_view packagePath)
    : m_hash(hash)
    , m_metadata(metadata)
    , m_nbImages(nbImages)
    , m_packageFs(packageFs)
    , m_packagePath(packagePath)
{
}

LayoutImagePackage Layout::getPackage() const
{
    return LayoutImagePackage(m_packageFs, m_packagePath.c_str());
}

bool Layout::operator==(const Layout& layout) const
{
    return m_hash == layout.m_hash && m_metadata == layout.m_metadata
           && &m_packageFs.get() == &layout.m_packageFs.get()
           && m_packagePath == layout.m_packagePath;
}