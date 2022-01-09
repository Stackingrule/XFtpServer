#include "XThread.h"
#include "XTask.h"

#include <thread>
#include <iostream>
#include <event2/event.h>
#include <unistd.h>

namespace xftp {

//激活线程任务的回调函数
static void NotifyCB(evutil_socket_t fd, short events, void *arg);

void XThread::Notify(evutil_socket_t fd, short which) {
    //水平触发 只要没有接受完成，会再次进来
    char buf[2] = { 0 };
    //linux中是管道，不能用recv
    int re = read(fd, buf, 1);

    if (re <= 0)
        return;
    std::cout << id << " thread " << buf << std::endl;
    XTask *task = nullptr;
    //获取任务，并初始化任务
    tasks_mutex.lock();
    if (tasks.empty()) {
        tasks_mutex.unlock();
        return;
    }
    task = tasks.front(); //先进先出
    tasks.pop_front();
    tasks_mutex.unlock();
    task->Init();
}

void XThread::AddTask(XTask *t) {
    if (!t)return;
    t->base = this->base;
    tasks_mutex.lock();
    tasks.push_back(t);
    tasks_mutex.unlock();
}

//线程激活
void XThread::Activate() {

    int re = write(this->notify_send_fd, "c", 1);

    if (re <= 0) {
        std::cerr << "XThread::Activate() failed!" << std::endl;
    }
}
//启动线程
void XThread::Start() {
    Setup();
    //启动线程
    std::thread th(&XThread::Main,this);

    //断开与主线程联系
    th.detach();
}

//安装线程，初始化event_base和管道监听事件用于激活
bool XThread::Setup() {
    //创建的管道 不能用send recv读取 read write
    int fds[2];
    if (pipe(fds)) {
        std::cerr << "pipe failed!" << std::endl;
        return false;
    }

    //读取绑定到event事件中，写入要保存
    notify_send_fd = fds[1];

    //创建libevent上下文（无锁）
    event_config *ev_conf = event_config_new();
    event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
    this->base = event_base_new_with_config(ev_conf);
    event_config_free(ev_conf);
    if (!base) {
        std::cerr << "event_base_new_with_config failed in thread!" << std::endl;
        return false;
    }

    //添加管道监听事件，用于激活线程执行任务
    event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
    event_add(ev, 0);

    return true;
}
//线程入口函数
void XThread::Main() {
    std::cout << id << " XThread::Main() begin" << std::endl;
    event_base_dispatch(base);
    event_base_free(base);

    std::cout << id << " XThread::Main() end" << std::endl;
}

static void NotifyCB(evutil_socket_t fd, short events, void *arg) {
    auto t = reinterpret_cast<XThread *>(arg);
    t->Notify(fd, events);
}

}