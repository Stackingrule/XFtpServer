#ifndef XFTPSERVER_XTASK_H
#define XFTPSERVER_XTASK_H

#include <iostream>
#include <string>
#include <event.h>

namespace xftp {

class XTask {

public:
    struct event_base *base = nullptr;
    int sock = 0;
    int thread_id = 0;

    /**
     * 初始化任务
     * @return
     */
    virtual bool Init() = 0;
};

}

#endif //XFTPSERVER_XTASK_H
