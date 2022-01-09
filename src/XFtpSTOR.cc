#include "XFtpSTOR.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
using namespace std;

namespace xftp {

//解析协议
void XFtpSTOR::Parse(const std::string &type, const std::string &msg) {
    int pos = msg.rfind(' ') + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string path = cmdTask->rootDir;
    path += cmdTask->curDir;
    path += filename;
    fp = fopen(path.c_str(), "wb");
    if (fp != nullptr) {
        ConnectPORT();
        ResponseCMD("150 File OK\r\n");
        bufferevent_trigger(cmdbev, EV_READ, 0);
    }
    else {
        ResponseCMD("450 file open failed!\r\n");
    }
}

void XFtpSTOR::Read(struct bufferevent *bev) {
    if (!fp)
        return;
    while (true) {
        int len = bufferevent_read(cmdbev, buf, sizeof((buf)));
        if (len <= 0)
            break;
        int size = fwrite(buf, 1, len, fp);
        std::cout << "[" << len << ":" << size << "]" << std::endl;
    }
}

void XFtpSTOR::Event(struct bufferevent *bev, short what) {
    //如果对方网络断掉，或者机器死机有可能收不到BEV_EVENT_EOF数据
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        std::cout << "XFTPSTOR BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT" << std::endl;
        Close();
        ResponseCMD("226 Transfer complete\r\n");
    } else if (what & BEV_EVENT_CONNECTED) {
        std::cout << "XFTPSTOR BEV_EVENT_CONNECTED" << std::endl;
    }
}

}