#ifndef XFTPSERVER_XFTPRETR_H
#define XFTPSERVER_XFTPRETR_H

#include "XFtpTask.h"

namespace xftp {

class XFtpRETR : public XFtpTask {
public:
    //解析协议
    void Parse(const std::string &type, const std::string &msg) override;

    void Write(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

private:
    FILE *fp = nullptr;
    char buf[1024] = {0};

};

}

#endif //XFTPSERVER_XFTPRETR_H
