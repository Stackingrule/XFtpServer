#ifndef XFTPSERVER_XFTPLIST_H
#define XFTPSERVER_XFTPLIST_H

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "XFtpTask.h"

namespace fs = std::filesystem;

namespace xftp {

class XFtpLIST : public XFtpTask {

public:
    //解析协议
    void Parse(const std::string &type, const std::string &msg) override;

    void Write(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

private:
    static std::string GetDirData(const std::string &path);

    static std::string GetFilePermissions(const fs::directory_entry &f);

    static std::string GetFileTime(const fs::directory_entry &f);

    static int GetDirectoryCount(const fs::path &fp);

    static std::string GetListData(std::string path);
};

}

#endif //XFTPSERVER_XFTPLIST_H
