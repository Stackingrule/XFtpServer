#include "XFtpServer.h"

namespace xftp {

XFtpServer::XFtpServer(int port, int thread_num) {
    //忽略管道信号，发送数据给已关闭的socket
    if (signal(SIGPIPE, SIG_IGN) == SIG_IGN) {
        return;
    }

    // 1.初始化线程池
    XThreadPool::Get()->Init(thread_num);

    base = event_base_new();
    if (base) {
        std::cout << "event_base_new success" << std::endl;
        std::cout << "XFtpServer Init success" << std::endl;
    }

    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);

    ev = evconnlistener_new_bind(
            base,
            listen_cb,
            base,
            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
            10,
            reinterpret_cast<sockaddr *>(&sock),
            sizeof(sock)
    );
}

void XFtpServer::Start() {
    if (base) {
        event_base_dispatch(base);
    }

    if (ev) {
        evconnlistener_free(ev);
    }
    if (base) {
        event_base_free(base);
    }
}

void XFtpServer::listen_cb(struct evconnlistener *e, int s, struct sockaddr *a, int socklen, void *arg) {
    std::cout << "listen_cb" << std::endl;
    XTask *task = XFtpFactory::Get()->CreateTask();
    task->sock = s;
    XThreadPool::Get()->Dispatch(task);
}

}