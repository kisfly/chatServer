#ifndef CHATSERVER_MAIN_H
#define CHATSERVER_MAIN_H


#include<unordered_map>
#include<functional>
#include<mutex>
#include"json.hpp"
#include"redis.hpp"
//#include"UserModel.hpp"
#include"friendmodel.hpp"
#include"groupmodel.hpp"
#include"offlinemessagemodel.hpp"
#include"RsaCrypto.h"
#include"AesCrypto.h"
#include"Base64.h"
#include"chatRedis.hpp"
#include<muduo/net/TcpServer.h>
using namespace muduo;
using namespace muduo::net;
using namespace std;


using json=nlohmann::json;

//处理消息的事件回调方法类型
using MsgHandler=function<void(const TcpConnectionPtr&, json&, Timestamp)>;
//聊天服务器业务类
class chatService
{
public:
    //获取单例对象的接口
    static chatService* getInstance();
    //处理登录业务
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //服务器异常，业务重置方法
    void reset();
    //添加好友
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //创建一个群聊
    void createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //加入群聊
    void addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //进行群聊天
    void groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //处理登录退出业务
    void loginout(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int userid, string msg);
    //处理客户端发送过来的密钥
    void handleKey(const TcpConnectionPtr& conn, json& js, Timestamp time);
    //返回对称加密的对象
    AesCrypto* getAesCrypto(const TcpConnectionPtr& conn);
    //使用对称加密的密钥加密数据,并返回加密后的数据
    string aesEncrypt(string data,AesCrypto* cipher);
    //对数据使用对称加密的密钥解密
    string aesDecrypt(string data,AesCrypto* cipher);

private:
    //构造函数私有化，防止在类外创建实例
    chatService();
    ~chatService()
    {
        //delete m_aescry;
    }

    //存储消息id和其对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    //数据操作类对象
    //UserModel _userModel;
    offlineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    //存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;
    //存储每一个客户端连接的密钥
    unordered_map<TcpConnectionPtr,AesCrypto*> _connKeyMap;

    //针对在线用户的通信连接的访问是一个多线程问题，但是STL是线程不安全的，所以需要使用互斥锁
    mutex _connMutex;//保证_userConnMap在多线程中安全
    // 保证_connKeyMap在多线程中安全
    mutex _keyMutex;

    // redis操作对象
    RedisHir _redis;

    myRedis m_redis;
};



#endif