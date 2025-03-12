#ifndef CHATSERVER_H
#define CHATSERVER_H

#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;


class chatServer
{
public:
    chatServer(EventLoop* loop,//事件循环
         const InetAddress& addr,//IP和端口
         const string& nameArg); //服务器名称

    //开启事件循环
    void start();
private:
    //专门处理用户的连接创建和断开
    void onConnection(const TcpConnectionPtr& conn);
    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr& conn,//tcp连接
        Buffer* buffer,                           //缓冲区
        Timestamp time);                         //接收到数据的时间信息

private:
    TcpServer _server;
    EventLoop* _loop;
};

#endif