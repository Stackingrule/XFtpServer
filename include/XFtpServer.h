#ifndef XFTPSERVER_XFTPSERVER_H
#define XFTPSERVER_XFTPSERVER_H

#include <iostream>
#include <csignal>
#include <cstring>
#include <event.h>
#include <event2/listener.h>

#include "XThreadPool.h"
#include "XFTPFactory.h"

namespace xftp {

class XFtpServer {

public:
    /**
     * @param port {端口}
     * @param thread_num  {线程数}
     */
    XFtpServer(int port, int thread_num);

    void Start();

private:
    sockaddr_in sock{};
    event_base* base;
    evconnlistener *ev;

   static void listen_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg);

};

}

#endif //XFTPSERVER_XFTPSERVER_H
