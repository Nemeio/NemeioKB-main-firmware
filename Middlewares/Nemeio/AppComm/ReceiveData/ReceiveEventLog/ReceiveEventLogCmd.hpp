#pragma once

#include "ReceiveDataFileListener.hpp"

class ReceiveEventLogCmd : public ReceiveDataFileListener
{
public:
    ReceiveEventLogCmd(fs::IFileSystem& fileSystem, std::string_view path);
    ~ReceiveEventLogCmd() final = default;

private:
    void doReceiveFileDone() final;
};
