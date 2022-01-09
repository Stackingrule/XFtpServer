#ifndef XFTPSERVER_XFTPPORT_H
#define XFTPSERVER_XFTPPORT_H

#include "XFtpTask.h"

namespace xftp {

class XFtpPORT : public XFtpTask {

public:
    void Parse(const std::string &type, const std::string &msg) override;
};

}

#endif //XFTPSERVER_XFTPPORT_H
