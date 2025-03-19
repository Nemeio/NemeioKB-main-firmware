#pragma once

#include "PackageFileSystem.hpp"
#include "ISignatureVerifier.hpp"
#include "ISharedBufferContainer.hpp"

class VerifiedPackageFileSystem : public PackageFileSystem
{
public:
    VerifiedPackageFileSystem(fs::IFileSystem& fileSystem,
                              std::string_view path,
                              ISignatureVerifier& signatureVerifier,
                              ISharedBufferContainer& sharedBufferContainer);
    ~VerifiedPackageFileSystem() final = default;

    int mount() override;

private:
    ISignatureVerifier& m_signatureVerifier;
    ISharedBufferContainer& m_sharedBufferContainer;

    bool verifySignature();
};
