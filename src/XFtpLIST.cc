#include "XFtpLIST.h"

#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>

namespace xftp {

//void XFtpLIST::Parse(const std::string &type, const std::string &msg) {
//    if (type == "PWD") {
//        std::string responseMsg = "257 \"";
//        responseMsg += cmdTask->curDir;
//        responseMsg += "\" is current dir.\r\n";
//        ResponseCMD(responseMsg);
//    } else if (type == "LIST") {
//        ///1.连接数据通道 2.回应150状态码 3.发送目录数据通道 4.回应226 5.关闭连接
//
//        ///命令通道回复消息
//        ///数据通道发送目录
//        //std::string dirData = "-rwxrwxrwx 1 root root 64463 Mar 14 09:53 101.jpg\r\n";
//        std::string dirData = GetDirData(cmdTask->rootDir + cmdTask->curDir);
//        ///1.连接数据通道
//        ConnectPORT();
//        ///2.回应150
//        ResCMD("150 Here comes the directory listing.\r\n");
//        ///3.数据通道发送数据
//        Send(dirData);
//    } else if (type == "CWD") {
//        int pos = msg.rfind(' ') + 1;
//        std::string path = msg.substr(pos, msg.size() - pos - 2);
//        if (path[0] == '/') {
//            cmdTask->curDir = path;
//        } else {
//            if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/') {
//                cmdTask += '/';
//            }
//            cmdTask->curDir += path + "/";
//        }
//        ResCMD("250 Directory success changed.\r\n");
//    } else if (type == "CDUP") {
//        std::string path = cmdTask->curDir;
//        if (path[path.size() - 1] == '/') {
//            path = path.substr(0, path.size() - 1);
//        }
//        int pos = path.rfind('/');
//        path = path.substr(0, pos);
//        cmdTask->curDir = path;
//        ResCMD("250 Directory success changed.\r\n");
//    }
//}

void XFtpLIST::Parse(const std::string &type, const std::string &msg) {
    std::string resmsg;

    if (type == "PWD") {
        //257 "/" is current directory.
        resmsg = "257 \"";
        resmsg += cmdTask->curDir;
        resmsg += "\" is current dir.\r\n";

        ResponseCMD(resmsg);
    }
    else if (type == "LIST") {
        //1连接数据通道 2 150 3 发送目录数据通道 4 发送完成226 5 关闭连接
        //命令通道回复消息 使用数据通道发送目录
        //-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
        //1 连接数据通道
        ConnectPORT();
        //2 1502 150
        ResponseCMD("150 Here comes the directory listing.\r\n");
        //string listdata = "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
        std::string listdata = GetListData(cmdTask->rootDir + cmdTask->curDir);
        //3 数据通道发送
        Send(listdata);
    }
    else if (type == "CWD") {
        //取出命令中的路径
        //CWD test\r\n
        //取出命令中的路径
        //CWD test\r\n
        int pos = msg.rfind(' ') + 1;
        //去掉结尾的\r\n
        std::string path = msg.substr(pos, msg.size() - pos - 2);
        if (path[0] == '/') {
            cmdTask->curDir = path;
        } else {
            if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
                cmdTask->curDir += "/";
            cmdTask->curDir += path + "/";
        }
        //  /test/
        ResponseCMD("250 Directory succes chanaged.\r\n");

        //cmdTask->curDir +=
    }
    else if (type == "CDUP") {
        //  /Debug/test_ser.A3C61E95.tlog /Debug   /Debug/
        std::string path = cmdTask->curDir;
        //统一去掉结尾的 /
        ////  /Debug/test_ser.A3C61E95.tlog /Debug
        if (path[path.size() - 1] == '/') {
            path = path.substr(0, path.size() - 1);
        }
        int pos = path.rfind('/');
        path = path.substr(0, pos);
        cmdTask->curDir = path;
        ResponseCMD("250 Directory succes chanaged.\r\n");
    }
}

void XFtpLIST::Write(bufferevent *bev) {
    ///4.发送完成 回应226
    ResponseCMD("226 Transfer complete\r\n");
    ///5.关闭连接
    Close();
}

void XFtpLIST::Event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT" << std::endl;
        Close();
    } else if (what & BEV_EVENT_CONNECTED) {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}

//std::string XFtpLIST::GetDirData(const std::string &path) {
//    std::cout << path << " " << fs::absolute(path) << std::endl;
//    std::string data;
//    for (auto &f:fs::directory_iterator(path)) {
//        auto &fp = f.path();
//        data += GetFilePermissions(f) + " ";
//        if (fs::is_directory(f)) {
//            data += std::to_string(GetDirectoryCount(fp)) + " " +
//                    std::to_string(4096) + " ";
//        } else {
//            data += std::to_string(1) + " " +
//                    std::to_string(f.file_size()) + " ";
//        }
//        data += GetFileTime(f) + " ";
//        data += fp.filename();
//        data += "\r\n";
//    }
//    return data;
//}
//
//std::string XFtpLIST::GetFilePermissions(const fs::directory_entry &f) {
//    fs::perms permission = f.status().permissions();
//    std::string permissions;
//    ///directory
//    permissions += f.is_directory() ? "d" : "-";
//    ///owner
//    permissions += (permission & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
//    permissions += (permission & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
//    permissions += (permission & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";
//    ///group
//    permissions += (permission & fs::perms::group_read) != fs::perms::none ? "r" : "-";
//    permissions += (permission & fs::perms::group_write) != fs::perms::none ? "w" : "-";
//    permissions += (permission & fs::perms::group_exec) != fs::perms::none ? "x" : "-";
//    ///others
//    permissions += (permission & fs::perms::others_read) != fs::perms::none ? "r" : "-";
//    permissions += (permission & fs::perms::others_write) != fs::perms::none ? "w" : "-";
//    permissions += (permission & fs::perms::others_exec) != fs::perms::none ? "x" : "-";
//
//    return permissions;
//}
//
//std::string XFtpLIST::GetFileTime(const fs::directory_entry &f) {
//    auto ftime = f.last_write_time();
//
//    std::time_t fileTime = std::chrono::system_clock::to_time_t(ftime);
//    std::stringstream ss;
//    ss << std::put_time(std::localtime(&fileTime), "%b %d %H:%M");
//    return ss.str();
//}
//
//int XFtpLIST::GetDirectoryCount(const fs::path &fp) {
//    int ans = 2;
//    for (auto &f:fs::directory_iterator(fp)) {
//        if (fs::is_directory(f)) {
//            ans++;
//        }
//    }
//    return ans;
//}

std::string XFtpLIST::GetListData(std::string path) {
    //-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
    std::string data = "";

    std::string cmd = "ls -l ";
    cmd += path;
    std::cout << "popen:" << cmd << std::endl;
    FILE *f = popen(cmd.c_str(), "r");
    if (!f)
        return data;
    char buffer[1024] = { 0 };
    for (;;)
    {
        int len = fread(buffer, 1, sizeof(buffer) - 1, f);
        if (len <= 0)break;
        buffer[len] = '\0';
        data += buffer;
    }
    pclose(f);

    return data;
}

}