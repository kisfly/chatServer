#include<functional>
#include<fstream>
#include<sstream>
#include"chatserver.hpp"
#include"json.hpp"
#include"public.hpp"
#include"chatservice.hpp"
#include"RsaCrypto.h"
#include"Base64.h"

#include<muduo/base/Logging.h>

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
    //生成非对称加密密钥对
    RsaCrypto* crypt=new RsaCrypto;
    crypt->generateRsaKey(RsaCrypto::BITS_2k);
    delete crypt;
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
        //用户成功连接
        LOG_INFO<<"publickey send client\n";
        sendRSAKey(conn);
    }
}
    
//专门处理用户的读写事件
void chatServer::onMessage(const TcpConnectionPtr& conn,//tcp连接
        Buffer* buffer,                           //缓冲区
        Timestamp time)                         //接收到数据的时间信息
{
    //获取客户端发送过来的信息
    string buf=buffer->retrieveAllAsString();
    //解密处理
    if(chatService::getInstance()->getAesCrypto(conn))
    {
        buf = chatService::getInstance()->aesDecrypt(buf,chatService::getInstance()->getAesCrypto(conn));
    }
    //数据的反序列化
    json js=json::parse(buf);

    //解耦网络模块和业务模块

    //根据消息的id找到对应的事件处理器
    int msgid=js["msgid"].get<int>();
    auto msgHandler=chatService::getInstance()->getHandler(msgid);
    //执行事件处理器
    msgHandler(conn, js, time);
}

//发送RSA公钥给客户端
void chatServer::sendRSAKey(const TcpConnectionPtr& conn)
{
    ifstream ifs("public.pem");
    if (!ifs.is_open())
    {
        LOG_ERROR << "open public.pem failed\n";
        return;
    }
    stringstream sstr;
    sstr<<ifs.rdbuf();
    string data=sstr.str();
    //发送数据
    json js;
    js["msgid"] = RSA_KEY_MSG;
    js["pubkey"] = data;
    //设置签名
    RsaCrypto rsa("private.pem",RsaCrypto::PrivateKey);
    js["signedData"]=rsa.sign(data);
    //发送数据
    conn->send(js.dump());

}