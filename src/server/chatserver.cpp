#include<functional>
#include<fstream>
#include<sstream>
#include"chatserver.hpp"
#include"json.hpp"
#include"public.hpp"
#include"chatservice.hpp"
#include"RsaCrypto.h"
#include"Base64.h"
#include"chatRedis.hpp"
#include"Logger.h"


using namespace std;
using namespace placeholders;
using json=nlohmann::json;

chatServer::chatServer(const std::string &ip,const uint16_t port,int subthreadnum,int workthreadnum)
                   :tcpserver_(ip,port,subthreadnum),threadpool_(workthreadnum,"WORKS")
{
    // 以下代码不是必须的，业务关心什么事件，就指定相应的回调函数。
    tcpserver_.setnewconnectioncb(bind(&chatServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.setcloseconnectioncb(bind(&chatServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.seterrorconnectioncb(bind(&chatServer::HandleError, this, std::placeholders::_1));
    tcpserver_.setonmessagecb(bind(&chatServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.setsendcompletecb(bind(&chatServer::HandleSendComplete, this, std::placeholders::_1));
   // tcpserver_.setremoveconnectioncb(bind(&chatServer::HandleRemove, this, std::placeholders::_1));
}

void chatServer::Start()
{
    generateRsaKeyAndSave();
    tcpserver_.start();
}

//发送RSA公钥给客户端
void chatServer::sendRSAKey(const spConnection& conn)
{
    myRedis m_redis;
    //连接redis++
    assert(m_redis.connectRedis());
    //从redis中获取公钥
    string data=m_redis.getRsaKey("publicKey");
    //发送数据
    json js;
    js["msgid"] = RSA_KEY_MSG;
    js["pubkey"] = data;
    //设置签名
    RsaCrypto rsa;  
    rsa.parseStringToKey(m_redis.getRsaKey("privateKey"),RsaCrypto::PrivateKey);
    js["signedData"]=rsa.sign(data);
    //发送数据
    LOG_INFO("js发送: %d",js.dump().size());
    conn->send(js.dump().c_str(),js.dump().size());    
}

//生成密钥对并保存到redis数据库中
void chatServer::generateRsaKeyAndSave()
{
    //生成非对称加密密钥对
    RsaCrypto* crypt=new RsaCrypto;
    crypt->generateRsaKey(RsaCrypto::BITS_2k);
    //保存公钥到redis
    ifstream ifs("public.pem");
    if (!ifs.is_open())
    {
        LOG_ERROR("open public.pem failed");
        return;
    }
    stringstream sstr;
    sstr<<ifs.rdbuf();
    string data=sstr.str();
    myRedis m_redis;
    //连接redis++
    assert(m_redis.connectRedis());
    //清空当前数据库
    m_redis.clearData();
    //插入公钥到redis中
    m_redis.insertRsaKey("publicKey", data);
    //保存私钥到redis
    ifs.close();
    ifs.open("private.pem");
    if (!ifs.is_open())
    {
        LOG_ERROR("open private.pem failed");
        return;
    }
    sstr.clear();
    sstr<<ifs.rdbuf();
    data=sstr.str();
    m_redis.insertRsaKey("privateKey", data);
    //关闭redis++
    //释放内存
    ifs.close();
    delete crypt;

    unlink("public.pem");
    unlink("private.pem"); 
}


// 停止服务。
void chatServer::Stop()
{
   // 停止工作线程。
   threadpool_.stop();
   LOG_INFO("工作线程已停止");

   // 停止IO线程（事件循环）。
   tcpserver_.stop();
   LOG_INFO("IO线程已停止");
}

// 处理新客户端连接请求，在TcpServer类中回调此函数。
void chatServer::HandleNewConnection(spConnection conn)    
{
   //用户成功连接
   LOG_INFO("publickey send client");
   sendRSAKey(conn);
}

// 关闭客户端的连接，在TcpServer类中回调此函数。 
void chatServer::HandleClose(spConnection conn)  
{
    //关闭连接
    chatService::getInstance()->clientCloseException(conn);
}

// 客户端的连接错误，在TcpServer类中回调此函数。
void chatServer::HandleError(spConnection conn)  
{
   HandleClose(conn);
}

// 处理客户端的请求报文，在TcpServer类中回调此函数。
void chatServer::HandleMessage(spConnection conn,std::string& message)     
{
   if (threadpool_.size()==0)
   {
       // 如果没有工作线程，表示在IO线程中计算。
       OnMessage(conn,message);       
   }
   else
   {
       // 把业务添加到线程池的任务队列中，交给工作线程去处理业务。
       threadpool_.addtask(bind(&chatServer::OnMessage,this,conn,message));
   }
}


// 处理客户端的请求报文，用于添加给线程池。
void chatServer::OnMessage(spConnection conn,std::string& message)     
{
    LOG_INFO("chatServer::OnMessage-处理客户端发送过来的数据");
    LOG_INFO("message: %d", message.data());
   //获取客户端发送过来的信息
   string buf=message;
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
   msgHandler(conn, js);
}

// 数据发送完成后，在TcpServer类中回调此函数。
void chatServer::HandleSendComplete(spConnection conn)     
{
   // std::cout << "Message send complete." << std::endl;
   // 根据业务的需求，在这里可以增加其它的代码。
}

void chatServer::HandleRemove(int fd)                       // 客户端的连接超时，在TcpServer类中回调此函数。
{
   LOG_INFO("fd(%d) 已超时");

   /*
   std::lock_guard<std::mutex> gd(mutex_);
   usermap_.erase(fd);   
   */                                 
}
