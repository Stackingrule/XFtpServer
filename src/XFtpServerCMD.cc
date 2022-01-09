#include "XFtpServerCMD.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include<iostream>
#include <cstring>
#include <string>

namespace xftp {

bool XFtpServerCMD::Init() {
    std::cout << "XFTPServerCMD::Init()" << std::endl;
    ///监听socket bufferevent
    auto bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
    if (bev == nullptr) {
        delete this;
        return false;
    }
    this->cmdbev = bev;
    this->SetCallback(bev);

    ///添加超时
    timeval readtime = {120, 0};
    bufferevent_set_timeouts(bev, &readtime, nullptr);

    std::string msg = "220 Welcome to libevent XFTPServer\r\n";
    bufferevent_write(bev, msg.c_str(), msg.size());

    return true;
}

void XFtpServerCMD::Read(bufferevent *bev) {
    char data[1024] = {0};
    while (true) {
        int len = bufferevent_read(bev, data, sizeof(data) - 1);
        if (len <= 0)
            break;
        data[len] = '\0';
        std::cout << "Recv CMD: " << data << std::flush;

        ///分发到处理对象

        ///分析类型
        std::string type;
        for (int i = 0; i < len; ++i) {
            if (data[i] == ' ' || data[i] == '\r')
                break;
            type += data[i];
        }
        std::cout << "Type: [" << type << "]" << std::endl;
        if (calls.find(type) != calls.end()) {
            auto task = calls[type];
            task->cmdTask = this;///用来处理回复命令和目录
            task->ip = ip;
            task->port = port;
            task->base = base;
            task->Parse(type, data);
            if (type == "PORT") {
                ip = task->ip;
                port = task->port;
            }
        } else {
            std::string msg = "200 OK\r\n";
            bufferevent_write(bev, msg.c_str(), msg.size());
        }
    }
}

void XFtpServerCMD::Event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT" << std::endl;
        delete this;
    }
}

void XFtpServerCMD::Reg(const std::string &cmd, XFtpTask *call) {
    if (cmd.empty()) {
        std::cout << "XFTPServerCMD::Reg cmd is empty" << std::endl;
        return;
    }
    if (!call) {
        std::cout << "XFTPServerCMD::Reg call is empty" << std::endl;
        return;
    }
    ///已经注册的不覆盖，提示错误
    if (calls.find(cmd) != calls.end()) {
        std::cout << cmd << " is already register " << std::endl;
        return;
    }
    calls[cmd] = call;
    calls_del[call] = 0;
}

XFtpServerCMD::~XFtpServerCMD() {
    Close();
    for (auto i : calls_del) {
        i.first->Close();
        delete i.first;
    }
}



}

