
#include "LinuxFileSystem.hpp"
#include <filesystem>
#include <fstream>

int LinuxFileSystem::remove(const std::string_view path)
{
    return std::filesystem::remove(path) ? fs::FS_ERR_OK : fs::FS_ERR_NOENT;
}

int LinuxFileSystem::remove_recursive(const std::string_view path)
{
    int fsRet = std::filesystem::remove_all(path);

    int ret;
    if(fsRet == 0)
    {
        ret = fs::FS_ERR_NOENT;
    }
    else if(fsRet > 0)
    {
        ret = fs::FS_ERR_OK;
    }
    else
    {
        ret = -1;
    }
    return ret;
}

int LinuxFileSystem::rename(std::string_view oldpath, std::string_view newpath)
{
    std::filesystem::rename(oldpath, newpath);

    return fs::FS_ERR_OK;
}

int LinuxFileSystem::stat(const std::string& path, fscommon::FSInfo& info)
{
    auto status = std::filesystem::status(path);
    std::filesystem::path fsPath(path);

    if(!std::filesystem::exists(status))
    {
        return fs::FS_ERR_NOENT;
    }

    fscommon::FSInfo retInfo(std::filesystem::is_directory(status) ? fs::FileType::DIR
                                                                   : fs::FileType::REG,
                             std::filesystem::file_size(path),
                             fsPath.stem().string());

    return fs::FS_ERR_OK;
}

int LinuxFileSystem::fileOpen(fscommon::FileHandle& file, std::string_view path, int flags)
{
    auto fsFlags = std::ios::binary;
    if(flags & fscommon::OpenFlag::RDONLY || flags & fscommon::OpenFlag::RDWR)
    {
        fsFlags |= std::ios::in;
    }
    if(flags & fscommon::OpenFlag::WRONLY || flags & fscommon::OpenFlag::RDWR)
    {
        fsFlags |= std::ios::out;
    }
    if(flags & fscommon::OpenFlag::TRUNC)
    {
        fsFlags |= std::ios::trunc;
    }
    if(flags & fscommon::OpenFlag::APPEND)
    {
        fsFlags |= std::ios::ate;
    }

    auto f = new std::fstream(path.data(), fsFlags);

    file = f->is_open() ? reinterpret_cast<fscommon::FileHandle>(f) : fs::INVALID_FILE_HANDLE;

    int ret;
    if(file != fs::INVALID_FILE_HANDLE)
    {
        ret = fs::FS_ERR_OK;
    }
    else
    {
        ret = fs::FS_ERR_NOENT;
        delete f;
    }

    return ret;
}
int LinuxFileSystem::fileClose(fscommon::FileHandle& file)
{
    if(file == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto f = reinterpret_cast<std::fstream*>(file);
    delete f;
    return fs::FS_ERR_OK;
}

int LinuxFileSystem::fileSync(fscommon::FileHandle& file)
{
    auto f = reinterpret_cast<std::fstream*>(file);
    f->flush();
    return fs::FS_ERR_OK;
}

ssize_t LinuxFileSystem::fileRead(fscommon::FileHandle& file, void* buffer, std::size_t size)
{
    if(file == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto f = reinterpret_cast<std::fstream*>(file);
    ssize_t realSize = fileSize(file);
    if(realSize < 0)
    {
        return -1;
    }

    f->read(reinterpret_cast<char*>(buffer), std::min(size, static_cast<size_t>(realSize)));

    return f->gcount();
}

ssize_t LinuxFileSystem::fileWrite(fscommon::FileHandle& file, const void* buffer, std::size_t size)
{
    if(file == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto f = reinterpret_cast<std::fstream*>(file);
    f->write(reinterpret_cast<const char*>(buffer), size);

    return size;
}

ssize_t LinuxFileSystem::fileSeek(fscommon::FileHandle& file, ssize_t off, fscommon::SeekFlag flag)
{
    std::ios_base::seekdir dir;

    if(file == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    switch(flag)
    {
    case fscommon::SeekFlag::ABSOLUTE_POS:
        dir = std::ios_base::beg;
        break;
    case fscommon::SeekFlag::CURRENT_POS:
        dir = std::ios_base::cur;
        break;
    case fscommon::SeekFlag::END:
        dir = std::ios_base::end;
        break;
    default:
        dir = std::ios_base::beg;
        break;
    }

    auto f = reinterpret_cast<std::fstream*>(file);
    f->seekp(off, dir);

    return f->tellp();
}

ssize_t LinuxFileSystem::fileSize(fscommon::FileHandle& file)
{
    if(file == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto f = reinterpret_cast<std::fstream*>(file);
    auto initPos = f->tellp();

    f->seekp(0, std::ios_base::end);
    ssize_t size = f->tellp();

    f->seekp(initPos, std::ios_base::beg);

    return size;
}
int LinuxFileSystem::mkdir(const char* path)
{
    return std::filesystem::create_directory(path) ? fs::FS_ERR_OK : -1;
}

std::string get_stem(const std::filesystem::path& p)
{
    return p.stem().string();
}

int LinuxFileSystem::dirOpen(fscommon::DirHandle& dir, std::string_view path)
{
    auto d = new std::filesystem::directory_iterator{path};

    dir = *d != std::filesystem::end(*d) ? reinterpret_cast<fscommon::FileHandle>(d)
                                         : fs::INVALID_DIR_HANDLE;
    int ret;

    if(dir != fs::INVALID_FILE_HANDLE)
    {
        ret = fs::FS_ERR_OK;
    }
    else
    {
        ret = fs::FS_ERR_NOENT;
        delete d;
    }

    return ret;
}

int LinuxFileSystem::dirClose(fscommon::DirHandle& dir)
{
    if(dir == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto d = reinterpret_cast<std::filesystem::directory_iterator*>(dir);
    delete d;

    return fs::FS_ERR_OK;
}

int LinuxFileSystem::dirRead(fscommon::DirHandle& dir, fscommon::FSInfo& info)
{
    if(dir == fs::INVALID_FILE_HANDLE)
    {
        return fs::FS_ERR_INVAL;
    }

    auto& d = *reinterpret_cast<std::filesystem::directory_iterator*>(dir);

    if(d == std::filesystem::end(d))
    {
        return 0;
    }

    fscommon::FSInfo retInfo(d->is_directory() ? fs::FileType::DIR : fs::FileType::REG,
                             d->is_directory() ? 0 : d->file_size(),
                             d->path().stem().string());
    ++d;

    info = retInfo;

    return 1;
}