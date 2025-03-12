#include"chatserver.hpp"
#include"json.hpp"
#include"chatservice.hpp"
#include<functional>
using namespace std;
using namespace placeholders;
using json=nlohmann::json;

chatServer::chatServer(EventLoop* loop, const InetAddress& addr, const string& nameArg)
:_server(loop, addr, nameArg),_loop(loop)
{
    //给服务器注册用户连接的创建和断开回调函数
    _server.setConnectionCallback(bind(&chatServer::onConnection, this, _1));        
    //给服务器注册用户读写事件回调
    _server.setMessageCallback(bind(&chatServer::onMessage, this, _1, _2, _3));
    //设置服务器端的线程数量,1个IO线程，3个工作线程
    _server.setThreadNum(4);
}

void chatServer::start()
{
    _server.start();
}

//专门处理用户的连接创建和断开
void chatServer::onConnection(const TcpConnectionPtr& conn)
{
    //客户端断开连接
    if(!conn->connected())
    {
        //关闭连接
        chatService::getInstance()->clientCloseException(conn);
        conn->shutdown();
        return;
    }
    else
    {

    }
}
    
//专门处理用户的读写事件
void chatServer::onMessage(const TcpConnectionPtr& conn,//tcp连接
        Buffer* buffer,                           //缓冲区
        Timestamp time)                         //接收到数据的时间信息
{
    //获取客户端发送过来的信息
    string buf=buffer->retrieveAllAsString();
    //数据的反序列化
    json js=json::parse(buf);

    //解耦网络模块和业务模块

    //根据消息的id找到对应的事件处理器
    int msgid=js["msgid"].get<int>();
    auto msgHandler=chatService::getInstance()->getHandler(msgid);
    //执行事件处理器
    msgHandler(conn, js, time);

}