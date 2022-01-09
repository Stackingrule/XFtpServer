#include "XFtpUSER.h"
#include <iostream>

namespace xftp {

void XFtpUSER::Parse(const std::string &type, const std::string &msg) {
    std::cout << "XFtpUSER::Parse " << type << " " << msg << std::endl;
    ResponseCMD("230 Login successful.\r\n");
}

}