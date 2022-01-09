#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpLIST.h"
#include "XFtpPORT.h"
#include "XFtpRETR.h"
#include "XFtpSTOR.h"

namespace xftp {

XFtpFactory *XFtpFactory::Get() {
    static XFtpFactory factory;
    return &factory;
}

XTask *XFtpFactory::CreateTask() {
    auto cmd = new XFtpServerCMD();
    ///注册ftp消息处理函数
    cmd->Reg("USER", new XFtpUSER());
    auto list = new XFtpLIST();
    cmd->Reg("PWD", list);
    cmd->Reg("LIST", list);
    cmd->Reg("CWD", list);
    cmd->Reg("CDUP", list);
    cmd->Reg("PORT", new XFtpPORT());
    cmd->Reg("RETR", new XFtpRETR());
    cmd->Reg("STOR", new XFtpSTOR());

    return cmd;
}

}
