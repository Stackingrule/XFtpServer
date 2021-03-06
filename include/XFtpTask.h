#ifndef XFTP_XFTPTASK_H
#define XFTP_XFTPTASK_H

#include "XTask.h"

#include <string>

namespace xftp {

class XFtpTask : public XTask {
public:
    std::string curDir = "/";
    std::string rootDir = "..";
    XFtpTask *cmdTask = nullptr;
    std::string ip = "";
    int port = 0;

    virtual void Read(bufferevent *bev) {}

    virtual void Write(bufferevent *bev) {}

    virtual void Event(bufferevent *bev, short what) {}

    void SetCallback(bufferevent *bev);

    bool Init() override;

    virtual void Parse(const std::string &type, const std::string &msg) {}

    void ResponseCMD(const std::string &msg) const;

    ///连接数据通道
    void ConnectPORT();

    ///用来发送建立了连接的数据通道
    void Send(const std::string &data);

    void Send(const char *data, int len);

    void Close();

protected:
    static void EventCB(bufferevent *bev, short what, void *arg);

    static void ReadCB(bufferevent *bev, void *arg);

    static void WriteCB(bufferevent *bev, void *arg);

    ///命令bev
    bufferevent *cmdbev = nullptr;
    FILE *fp = nullptr;

};

}
#endif //XFTP_XFTPTASK_H
