#include "ReceiveEventLogCmd.hpp"

ReceiveEventLogCmd::ReceiveEventLogCmd(fs::IFileSystem& fileSystem, std::string_view path)
    : ReceiveDataFileListener(AppCommDefs::RCVDATAID_SYSFAILLOG, fileSystem, path)
{
}

void ReceiveEventLogCmd::doReceiveFileDone()
{
    mFileSystem.remove(mPath.c_str());
}