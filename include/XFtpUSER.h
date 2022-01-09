#ifndef XFTPSERVER_XFTPUSER_H
#define XFTPSERVER_XFTPUSER_H

#include "XFtpTask.h"

namespace xftp {

class XFtpUSER : public XFtpTask {
public:
    //解析协议
    void Parse(const std::string &type, const std::string &msg) override;

    XFtpUSER() = default;

    ~XFtpUSER() = default;
};

}

#endif //XFTPSERVER_XFTPUSER_H
