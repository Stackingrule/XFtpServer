#include "XThreadPool.h"

namespace xftp {

// 初始化所有线程并启动线程
void XThreadPool::Init(int threadCnt) {
    this->threadCount = threadCnt;
    this->lastThread = -1;
    for (int i = 0; i < threadCnt; ++i) {
        XThread* t = new XThread();
        t->id = i + 1;
        std::cout << "Create thread " << i << std::endl;
        // 启动线程
        t->Start();
        threads.push_back(t);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void XThreadPool::Dispatch(XTask *task) {
    //轮询
    if (!task)return;
    int tid = (lastThread + 1) % threadCount;
    lastThread = tid;
    XThread *t = threads[tid];

    t->AddTask(task);

    //激活线程
    t->Activate();
}

}