#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"
using namespace mynetwork;
class chatServer
{
public:
    //需要ip和端口号，以及io线程和工作线程的数量
    chatServer(const std::string &ip,const uint16_t port,
               int subthreadnum=3,int workthreadnum=5);

    //开启事件循环
    void Start();
    //停止服务
    void Stop();
private:
    void HandleNewConnection(spConnection conn);     // 处理新客户端连接请求，在TcpServer类中回调此函数。
    void HandleClose(spConnection conn);                      // 关闭客户端的连接，在TcpServer类中回调此函数。 
    void HandleError(spConnection conn);                       // 客户端的连接错误，在TcpServer类中回调此函数。
    void HandleMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，在TcpServer类中回调此函数。
    void HandleSendComplete(spConnection conn);        // 数据发送完成后，在TcpServer类中回调此函数。   

    void OnMessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，用于添加给线程池。
    void HandleRemove(int fd);               //连接超时，服务端进行处理
    //发送公钥给客户端
    void sendRSAKey(const spConnection& conn);    
    //生成密钥对并保存到redis数据库中
    void generateRsaKeyAndSave();    

private:
    TcpServer tcpserver_;
    ThreadPool threadpool_;                        // 工作线程池。
};

#endif