#ifndef CHATSERVER_MAIN_H
#define CHATSERVER_MAIN_H


#include<unordered_map>
#include<functional>
#include<mutex>
#include"json.hpp"
#include"redis.hpp"
#include"friendmodel.hpp"
#include"groupmodel.hpp"
#include"offlinemessagemodel.hpp"
#include"RsaCrypto.h"
#include"AesCrypto.h"
#include"Base64.h"
#include"chatRedis.hpp"
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"


using namespace std;
//占位符的命令空间
using namespace placeholders;
using json=nlohmann::json;
using namespace mynetwork;

//处理消息的事件回调方法类型
using MsgHandler=function<void(const spConnection&, json&)>;
//聊天服务器业务类
class chatService
{
public:
    //获取单例对象的接口
    static chatService* getInstance();
    //处理登录业务
    void login(const spConnection& conn, json& js);
    //处理注册业务
    void reg(const spConnection& conn, json& js);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理客户端异常退出
    void clientCloseException(const spConnection& conn);
    //一对一聊天业务
    void oneChat(const spConnection& conn, json& js);
    //服务器异常，业务重置方法
    void reset();
    //添加好友
    void addFriend(const spConnection& conn, json& js);
    //创建一个群聊
    void createGroup(const spConnection& conn, json& js);
    //加入群聊
    void addGroup(const spConnection& conn, json& js);
    //进行群聊天
    void groupChat(const spConnection& conn, json& js);
    //处理登录退出业务
    void loginout(const spConnection& conn, json& js);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int userid, string msg);
    //处理客户端发送过来的密钥
    void handleKey(const spConnection& conn, json& js);
    //返回对称加密的对象
    AesCrypto* getAesCrypto(const spConnection& conn);
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
    unordered_map<int, spConnection> _userConnMap;
    //存储每一个客户端连接的密钥
    unordered_map<spConnection,AesCrypto*> _connKeyMap;

    //针对在线用户的通信连接的访问是一个多线程问题，但是STL是线程不安全的，所以需要使用互斥锁
    mutex _connMutex;//保证_userConnMap在多线程中安全
    // 保证_connKeyMap在多线程中安全
    mutex _keyMutex;

    // redis操作对象
    RedisHir _redis;

    myRedis m_redis;
};



#endif