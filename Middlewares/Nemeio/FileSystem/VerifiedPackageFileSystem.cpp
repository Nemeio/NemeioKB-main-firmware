#include "fs.hpp"
#include "VerifiedPackageFileSystem.hpp"
#include "SharedBufferIds.hpp"

using namespace fscommon;

VerifiedPackageFileSystem::VerifiedPackageFileSystem(fs::IFileSystem& fileSystem,
                                                     std::string_view path,
                                                     ISignatureVerifier& signatureVerifier,
                                                     ISharedBufferContainer& sharedBufferContainer)
    : PackageFileSystem(fileSystem, path)
    , m_signatureVerifier(signatureVerifier)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

int VerifiedPackageFileSystem::mount()
{
    AutoLock autoLock(mMutex);

    int err = PackageFileSystem::mount();

    if(err == FS_ERR_OK)
    {
        bool success = verifySignature();
        if(!success)
        {
            err = FS_ERR_SECURITY;
        }
    }

    if(err != FS_ERR_OK)
    {
        unmount();
    }

    return err;
}

bool VerifiedPackageFileSystem::verifySignature()
{
    bool success = m_signatureVerifier.init();

    // First, append the metadata.
    // The signature in metadata must be set to 0 for the verification
    if(success)
    {
        s_globalHeader globalHeader(mFirmwarePackageMetadata.global);
        memset(globalHeader.latest.signature, 0, sizeof(globalHeader.latest.signature));
        etl::array_view<const std::byte> globalHeaderView{reinterpret_cast<const std::byte*>(
                                                              &globalHeader),
                                                          sizeof(globalHeader)};
        success = m_signatureVerifier.appendDataToVerify(globalHeaderView);
    }

    if(success)
    {
        size_t sizeFirmwares = sizeof(s_FirmwareHeader)
                               * mFirmwarePackageMetadata.global.latest.firmwareNumber;
        etl::array_view<const std::byte> firmwaresHeaderView{reinterpret_cast<const std::byte*>(
                                                                 mFirmwarePackageMetadata.firmwares),
                                                             sizeFirmwares};
        success = m_signatureVerifier.appendDataToVerify(firmwaresHeaderView);
    }

    if(success)
    {
        ISharedBuffer& buffer = m_sharedBufferContainer.getSharedBuffer(
            shared_buffer_id::PKG_DIGEST_COMPUTE_SHARED_BUFFER_ID);
        AutoLock autoLock(buffer);

        size_t remaining = mFirmwarePackageMetadata.global.common.size
                           - mFirmwarePackageFile.tell();

        while(remaining && success)
        {
            ssize_t readSize = mFirmwarePackageFile.read(buffer.buffer().data(),
                                                         etl::min(remaining,
                                                                  buffer.buffer().size()));

            success = readSize > 0;

            if(success)
            {
                remaining -= readSize;
                success = m_signatureVerifier.appendDataToVerify(
                    etl::array_view<const std::byte>{buffer.buffer().begin(), readSize});
            }
        }
    }

    if(success)
    {
        etl::array_view<const std::byte>
            signatureView{reinterpret_cast<const std::byte*>(
                              mFirmwarePackageMetadata.global.latest.signature),
                          sizeof(mFirmwarePackageMetadata.global.latest.signature)};
        success = m_signatureVerifier.verify(signatureView);
    }

    return success;
}