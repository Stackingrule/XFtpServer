#ifndef XFTPSERVER_XFTPSTOR_H
#define XFTPSERVER_XFTPSTOR_H

#include "XFtpTask.h"

namespace xftp {

class XFtpSTOR : public XFtpTask {

public:
    void Parse(const std::string &type, const std::string &msg) override;

    void Read(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

private:
    FILE *fp = nullptr;
    char buf[1024] = {0};

};

}

#endif //XFTPSERVER_XFTPSTOR_H
