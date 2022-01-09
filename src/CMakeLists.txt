# aux_source_directory(. SRC_LIST)
set(
        SRC_LIST

        XFtpTask.cc
        XThread.cc
        XFtpFactory.cc
        XThreadPool.cc
        XFtpSTOR.cc
        XFtpRETR.cc
        XFtpPORT.cc
        XFtpLIST.cc
        XFtpUSER.cc
        XFtpServerCMD.cc

        XFtpServer.cc
)

add_library(XFtpServer ${SRC_LIST})
target_link_libraries(XFtpServer event pthread)