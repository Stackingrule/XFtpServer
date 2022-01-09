cmake_minimum_required(VERSION 3.22.1)
project(XFtpServer)

set(CMAKE_CXX_STANDARD 17)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/)


# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/include/)

# 设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib/)

add_subdirectory(src)
#add_executable(XFtpServer xftp_server.cpp ./src/XFtpTask.cc ./src/XThreadPool.cc ./src/XFtpFactory.cc ./src/XThread.cc ./src/XFtpServerCMD.cc ./src/XFtpUSER.cc ./src/XFtpLIST.cc ./src/XFtpPORT.cc ./src/XFtpRETR.cc ./src/XFtpSTOR.cc)
#target_link_libraries(XFtpServer event pthread)
add_executable(FtpServer xftp_server.cc)
target_link_libraries(FtpServer XFtpServer)