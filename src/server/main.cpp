#include"chatserver.hpp"
#include"chatservice.hpp"
#include<iostream>
#include<signal.h>
using namespace std;

//处理服务器ctrl+c结束后，重置user的状态
void resetHanlder(int signo) 
{
    // 重置SIGINT信号的处理函数
    cout << "Caught SIGINT, exiting..." << endl;
    chatService::getInstance()->reset();  // 重置user的状态
    exit(0);  // 0表示正常退出，非0表示非正常退出
}

int main(int argc, char *argv[]) {

    signal(SIGINT, resetHanlder);  //处理SIGINT信号，防止程序结束时数据库出现错误

    if(argc<3)
    {
        cerr<<"Usage: "<<argv[0]<<" ip port"<<endl;
        exit(-1);
    }
    //解析命令行参数传递的ip和port
    char *ip=argv[1];
    uint16_t port=atoi(argv[2]);
    //创建EventLoop事件循环对象的指针
    EventLoop loop;
    //创建InetAddress对象的IP和端口
    InetAddress addr(ip,port);
    //创建chatServer的实例
    chatServer server(&loop, addr, "ChatServer");
    //开启事件循环
    server.start();
    //让事件循环一直运行
    loop.loop();  //类似调用epoll_wait函数,以阻塞方式的等待新用户连接，已连接用户的读写事件等

    return 0;
}