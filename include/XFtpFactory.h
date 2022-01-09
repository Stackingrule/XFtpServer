#ifndef XFTP_XFTPFACTORY_H
#define XFTP_XFTPFACTORY_H

#include "XTask.h"

#include <event2/event.h>

namespace xftp {

class XFtpFactory {
public:

    static XFtpFactory* Get();

    XTask *CreateTask();

private:

    XFtpFactory() = default;
};

}

#endif //XFTP_XFTPFACTORY_H
