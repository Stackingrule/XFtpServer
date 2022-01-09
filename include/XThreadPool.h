#ifndef XFTP_XTHREADPOOL_H
#define XFTP_XTHREADPOOL_H

#include <vector>
#include <thread>
#include <iostream>

#include "XTask.h"

#include "XThread.h"

namespace xftp {

class XThreadPool {
public:
    static XThreadPool *Get() {
        static XThreadPool threadPool;
        return &threadPool;
    }

    ~XThreadPool() = default;

    void Init(int num);

    void Dispatch(XTask *task);

private:
    XThreadPool() = default;

    int threadCount = 0;
    int lastThread = -1;
    std::vector<XThread *> threads;
};

}

#endif //XFTP_XTHREADPOOL_H
