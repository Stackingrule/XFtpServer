#include "XFtpServer.h"

using namespace xftp;

int main() {
    XFtpServer ftpServer(21, 10);
    ftpServer.Start();

}