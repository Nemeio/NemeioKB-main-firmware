/*
 * ConfigurationDisplayState.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_CONFIGURATIONDISPLAYVARIANT_HPP_
#define DISPLAY_DISPLAYSTATE_CONFIGURATIONDISPLAYVARIANT_HPP_

#include "Configuration.hpp"
#include "MainDisplayVariant.hpp"
#include "CompressedFileObject.hpp"
#include "etl/variant.h"
#include "ScreenDefs.hpp"
#include <functional>
#include "DefaultLayoutModifierStrategy.hpp"
#include "MacLayoutModifierStrategy.hpp"
#include "embedded_ostream.hpp"
#include "Configuration.hpp"
#include "PackageFileSystem.hpp"
#include "CompressedFileObject.hpp"
#include "FileObject.hpp"
#include "ConfigurationDisplayInfo.hpp"

template<std::size_t READ_FILE_BUFFER>
class ConfigurationDisplayVariant : public MainDisplayVariant
{
public:
    ConfigurationDisplayVariant(const ConfigurationDisplayInfo& configurationDisplayInfo,
                                StreamDataReadTask& streamDataReadTask,
                                std::array<uint8_t, READ_FILE_BUFFER>& configurationReadFileBuffer);
    virtual ~ConfigurationDisplayVariant() = default;
    void draw(IScreen& screen, uint8_t fbNum) final;

    const ILayoutModifierStrategy& getModifierStrategy() const;

    bool operator==(const ConfigurationDisplayVariant& variant) const;

private:
    ConfigurationDisplayInfo mConfigurationDisplayInfo;
    std::reference_wrapper<StreamDataReadTask> mStreamDataReadTask;
    std::reference_wrapper<std::array<uint8_t, READ_FILE_BUFFER>> mConfigurationReadFileBuffer;

    // Use common buffers for draw and upgrade as they will not be processed at the same time
    static constexpr std::size_t MAX_DRAW_BUFFER_SIZE = 1496;
    static constexpr std::size_t CONFIGURATION_STREAM_BUFFER_SIZE = MAX_DRAW_BUFFER_SIZE * 2;
    static constexpr int32_t STREAM_TIMEOUT_MS = 250;

    static constexpr std::size_t UPGRADE_READ_BUFFER_SIZE = 256;
    static constexpr std::size_t UPGRADE_STREAM_BUFFER_SIZE = UPGRADE_READ_BUFFER_SIZE;

    static constexpr std::size_t READ_FROM_STREAM_BUFFER_SIZE = std::max(MAX_DRAW_BUFFER_SIZE,
                                                                         UPGRADE_READ_BUFFER_SIZE);
    static constexpr std::size_t STREAM_BUFFER_SIZE = std::max(CONFIGURATION_STREAM_BUFFER_SIZE,
                                                               UPGRADE_STREAM_BUFFER_SIZE);
    static constexpr std::size_t SOURCE_READ_CHUNK_BUFFER_SIZE = READ_FROM_STREAM_BUFFER_SIZE;

    /* FIXME Displayer is using stack, so we must declare these as static */
    static std::array<uint8_t, READ_FROM_STREAM_BUFFER_SIZE> mReadFromStreamBuffer;
    static std::array<uint8_t, STREAM_BUFFER_SIZE> mStreamBufferStorage;
    static std::array<uint8_t, SOURCE_READ_CHUNK_BUFFER_SIZE> mSourceReadChunkBuffer;

    /* FIXME Handle modifier strategy outside displayer */
    etl::variant<NoLayoutModifierStrategy, DefaultLayoutModifierStrategy, MacLayoutModifierStrategy>
        mModifierStrategy;

    void doDraw(IScreen& screen);
    void drawJpeg(IScreen& screen, const configuration::Layout& layout);
    void drawJpegImagePackage(IScreen& screen,
                              configuration::LayoutImagePackage& imagePackage,
                              ScreenDefs::BackgroundColor backgroundColor);

    void setModifierStrategy(size_t nbImages);
};

template<std::size_t READ_FILE_BUFFER>
std::array<uint8_t, ConfigurationDisplayVariant<READ_FILE_BUFFER>::READ_FROM_STREAM_BUFFER_SIZE>
    ConfigurationDisplayVariant<READ_FILE_BUFFER>::mReadFromStreamBuffer;
template<std::size_t READ_FILE_BUFFER>
std::array<uint8_t, ConfigurationDisplayVariant<READ_FILE_BUFFER>::STREAM_BUFFER_SIZE>
    ConfigurationDisplayVariant<READ_FILE_BUFFER>::mStreamBufferStorage;
template<std::size_t READ_FILE_BUFFER>
std::array<uint8_t, ConfigurationDisplayVariant<READ_FILE_BUFFER>::SOURCE_READ_CHUNK_BUFFER_SIZE>
    ConfigurationDisplayVariant<READ_FILE_BUFFER>::mSourceReadChunkBuffer;

template<std::size_t READ_FILE_BUFFER>
ConfigurationDisplayVariant<READ_FILE_BUFFER>::ConfigurationDisplayVariant(
    const ConfigurationDisplayInfo& configurationDisplayInfo,
    StreamDataReadTask& streamDataReadTask,
    std::array<uint8_t, READ_FILE_BUFFER>& configurationReadFileBuffer)
    : MainDisplayVariant(MainDisplayVariantId::CONFIGURATION)
    , mConfigurationDisplayInfo(configurationDisplayInfo)
    , mStreamDataReadTask(streamDataReadTask)
    , mConfigurationReadFileBuffer(configurationReadFileBuffer)
{
    if(mConfigurationDisplayInfo.layout.has_value())
    {
        setModifierStrategy(mConfigurationDisplayInfo.layout.value().getNbImages());
    }
}

template<std::size_t READ_FILE_BUFFER>
void ConfigurationDisplayVariant<READ_FILE_BUFFER>::setModifierStrategy(size_t nbImages)
{
    switch(nbImages)
    {
    case 1:
        mModifierStrategy = NoLayoutModifierStrategy();
        break;
    case 6:
        if(mConfigurationDisplayInfo.operatingSystem == configuration::OperatingSystem::MAC)
        {
            mModifierStrategy = MacLayoutModifierStrategy();
        }
        else
        {
            mModifierStrategy = DefaultLayoutModifierStrategy();
        }
        break;
    default:
        break;
    }
}

template<std::size_t READ_FILE_BUFFER>
const ILayoutModifierStrategy& ConfigurationDisplayVariant<READ_FILE_BUFFER>::getModifierStrategy()
    const
{
    return castToBase<ILayoutModifierStrategy>(mModifierStrategy);
}

template<std::size_t READ_FILE_BUFFER>
bool ConfigurationDisplayVariant<READ_FILE_BUFFER>::operator==(
    const ConfigurationDisplayVariant& variant) const
{
    return variant.mConfigurationDisplayInfo == mConfigurationDisplayInfo;
}

template<std::size_t READ_FILE_BUFFER>
void ConfigurationDisplayVariant<READ_FILE_BUFFER>::draw(IScreen& screen, uint8_t fb)
{
    doDraw(screen);
}

template<std::size_t READ_FILE_BUFFER>
void ConfigurationDisplayVariant<READ_FILE_BUFFER>::doDraw(IScreen& screen)
{
    if(mConfigurationDisplayInfo.layout.has_value())
    {
        auto& layout = mConfigurationDisplayInfo.layout.value();
        if(layout.getMetadata().format == ScreenDefs::PixelFormat::_JPEG)
        {
            drawJpeg(screen, layout);
        }
    }
}

template<std::size_t READ_FILE_BUFFER>
void ConfigurationDisplayVariant<READ_FILE_BUFFER>::drawJpeg(IScreen& screen,
                                                             const configuration::Layout& layout)
{
    configuration::LayoutImagePackage layoutImagePackage = layout.getPackage();

    drawJpegImagePackage(screen, layoutImagePackage, layout.getMetadata().background);
}

template<std::size_t READ_FILE_BUFFER>
void ConfigurationDisplayVariant<READ_FILE_BUFFER>::drawJpegImagePackage(
    IScreen& screen,
    configuration::LayoutImagePackage& imagePackage,
    ScreenDefs::BackgroundColor backgroundColor)
{
    ScreenDefs::ScreenErrCode ret = ScreenDefs::ScreenErrCode::SUCCESS;
    size_t imageIdx = 0;

    auto initCb = [](ISizeReader& reader) { return true; };

    auto readStreamCb = [&ret, &screen, &imageIdx, backgroundColor](IStreamDataReader& reader)
    {
        auto image = Image(Image::eImageType::JPEG, reader);

        IFrameBuffer& buffer = screen.getAssociatedScreenController()
                                   .getFrameBuffer(imageIdx, image.getType());
        ret = (buffer.loadImage(image) ? ScreenDefs::ScreenErrCode::SUCCESS
                                       : ScreenDefs::ScreenErrCode::UNEXPECTED);

        /* Display first image */
        if(IScreenController::DisplayOptions displayOptions{eDisplayMode::FULL, backgroundColor};
           imageIdx == 0 && ret == ScreenDefs::ScreenErrCode::SUCCESS
           && ScreenDefs::ScreenErrCode::SUCCESS
                  != screen.getAssociatedScreenController().display(screen, buffer, displayOptions))
        {
            ret = ScreenDefs::ScreenErrCode::UNEXPECTED;
        }

        return (ScreenDefs::ScreenErrCode::SUCCESS == ret);
    };

    int fsErr = imagePackage.getPackageFileSystem().mount();

    if(fsErr == fs::FSErrCode::FS_ERR_OK)
    {
        size_t nbImages = imagePackage.getNbImages();

        for(imageIdx = 0; imageIdx < nbImages; imageIdx++)
        {
            etl::string<fs::MAX_PATH_SIZE> imagePath;
            imagePackage.getImagePath(imageIdx, imagePath);
            FileObject<READ_FILE_BUFFER> fileObject(mStreamDataReadTask,
                                                    imagePackage.getPackageFileSystem(),
                                                    imagePath.c_str(),
                                                    mConfigurationReadFileBuffer);

            fileObject
                .template read<READ_FROM_STREAM_BUFFER_SIZE, STREAM_BUFFER_SIZE, STREAM_TIMEOUT_MS>(
                    ConfigurationDisplayVariant::mReadFromStreamBuffer,
                    ConfigurationDisplayVariant::mStreamBufferStorage,
                    1496,
                    ConfigurationDisplayVariant::mSourceReadChunkBuffer,
                    initCb,
                    readStreamCb);
        }
    }
}

#endif /* DISPLAY_DISPLAYSTATE_CONFIGURATIONDISPLAYVARIANT_HPP_ */
