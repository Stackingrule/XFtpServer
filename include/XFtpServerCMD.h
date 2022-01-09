#ifndef XFTP_XFTPSERVERCMD_H
#define XFTP_XFTPSERVERCMD_H

#include "XFtpTask.h"

#include <map>

namespace xftp {

class XFtpServerCMD :public XFtpTask {
public:
    //初始化任务
    bool Init() override;

    void Read(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

    //注册命令处理对象 不需要考虑线程安全，调用时还未分发到线程
    void Reg(const std::string &cmd, XFtpTask *call);

    XFtpServerCMD() = default;

    ~XFtpServerCMD();


private:
    //注册的处理对象
    std::map<std::string, XFtpTask*>calls;
    //用来做空间清理
    std::map<XFtpTask*, int> calls_del;

};

}

#endif //XFTP_XFTPSERVERCMD_H