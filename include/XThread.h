#ifndef XFTP_XTHREAD_H
#define XFTP_XTHREAD_H

#include "XTask.h"

#include <event2/event.h>
#include <list>
#include <mutex>

namespace xftp {

class XThread {
public:
    XThread() = default;

    ~XThread() = default;

    ///
    void Start();

    ///
    void Main();

    ///
    /// \return
    bool Setup();

    /// 收到主线程发出的激活消息（线程池的分发）
    /// \param fd
    /// \param which
    void Notify(evutil_socket_t fd, short which);

    ///
    void Activate();

    /// 添加处理的任务，一个线程同事可以处理多个任务，共用一个event_base
    /// \param task
    void AddTask(XTask *task);

    int id = 0;

private:
    int notify_send_fd = 0;
    event_base *base = nullptr;
    ///任务列表
    std::list<XTask *> tasks;
    std::mutex tasks_mutex;
};

}

#endif //XFTP_XTHREAD_H
