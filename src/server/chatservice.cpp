#include<vector>
#include"chatservice.hpp"
#include"public.hpp"
#include<muduo/base/Logging.h>


using namespace muduo;

chatService* chatService::getInstance()
{
    static chatService service;
    return &service;
}

chatService::chatService()
{
    // 注册消息id和其对应的处理方法
    _msgHandlerMap[LOGIN_MSG] = bind(&chatService::login, this, _1, _2, _3);
    _msgHandlerMap[LOGINOUT_MSG]=bind(&chatService::loginout,this,_1,_2,_3);
    _msgHandlerMap[REG_MSG] = bind(&chatService::reg, this, _1, _2, _3);
    _msgHandlerMap[ONE_CHAT_MSG] = bind(&chatService::oneChat, this, _1, _2, _3);
    _msgHandlerMap[ADD_FRIEND_MSG] = bind(&chatService::addFriend, this, _1, _2, _3);
    _msgHandlerMap[CREATE_GROUP_MSG] = bind(&chatService::createGroup, this, _1, _2, _3);
    _msgHandlerMap[ADD_GROUP_MSG] = bind(&chatService::addGroup, this, _1, _2, _3);
    _msgHandlerMap[GROUP_CHAT_MSG] = bind(&chatService::groupChat, this, _1, _2, _3);
    _msgHandlerMap[AES_KEY_MSG]=bind(&chatService::handleKey, this,_1,_2,_3);

     // 连接redis服务器
     if (_redis.connect())
     {
         // 设置上报消息的回调
         _redis.init_notify_handler(bind(&chatService::handleRedisSubscribeMessage, this, _1, _2));
     }
}

//获取消息对应的处理器
MsgHandler chatService::getHandler(int msgid)
{
    //记录错误日志
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end()) 
    {
        return [=](const TcpConnectionPtr&, const string&, Timestamp) 
        {
            LOG_ERROR << "can not find handler for msgid: " << msgid;
        };
    }
    return it->second;
}

//处理登录业务
void chatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    //获取js中的数据
    int id=js["id"].get<int>();
    string pwd=js["password"];
    User user =_userModel.query(id);
    string sendbuf;
    //id号和密码都正确-登录成功
    if(user.getId()==id && user.getPassword()==pwd)
    {
        //该用户已经登录-不能重复登录
        if(user.getState()=="online")
        {
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
            response["errmsg"] = "该用户已在线";//返回的错误信息
            //根据conn在哈希表中找到AesCrypto            
            {
                lock_guard<mutex> lock(_keyMutex);      
                //获取conn在哈希表中找到AesCrypto
                auto it = _connKeyMap.find(conn);
                if(it!=_connKeyMap.end())
                {
                    sendbuf=aesEncrypt(response.dump(),it->second);
                }
                else
                {
                    LOG_ERROR << "can not find aeskey for conn: " ;
                }
            }            
            conn->send(sendbuf.c_str(),sendbuf.size());
            LOG_INFO << "user is already online, name: " << user.getName()<<"\n";
            return;
        }

        //登录成功-记录该用户和服务器端的tcp连接-使用互斥锁
        {
            //使用大括号表示这个互斥锁的声明周期只在这个大括号内
            lock_guard<mutex> lock(_connMutex);
            _userConnMap[id] = conn;
        }
        
        //登录成功-更新用户状态
        user.setState("online");
        _userModel.updateState(user);     
        
        //订阅通道
        _redis.subscribe(id);
        
        // 创建一个新的js对象，序列化消息并发送
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["id"] = id;
        response["errno"] = 0;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        response["name"] = user.getName();

        //查询该用户是否有离线消息
        vector<string> offlineMsg = _offlineMsgModel.query(id);

        if(!offlineMsg.empty())
        {
            response["offlinemsg"] = offlineMsg;
            //删除离线消息
            _offlineMsgModel.remove(id);
        }

        //查询该用户的好友信息并返回
        vector<User> friends = _friendModel.query(id);
        if(!friends.empty())
        {
            // 构建friends数组
            json friendss = json::array();
            for(auto& friend_ : friends)
            {
                json friendJson;
                friendJson["id"] = friend_.getId();
                friendJson["name"] = friend_.getName();
                friendJson["state"] = friend_.getState();
                friendss.push_back(friendJson);
            }
            response["friends"] = friendss;
        }

        //查询该用户加入的群组信息并返回
        vector<Group> groups = _groupModel.queryGroups(id);
        if(!groups.empty())
        {
            // 构建group数组
            json groupArray1 = json::array();
            for(auto& group_ : groups)
            {
                json groupJson;
                groupJson["id"] = group_.getId();
                groupJson["groupname"] = group_.getName();
                groupJson["groupdesc"] = group_.getDesc();
                //将vector<GroupUser>数据转为json数据
                json userNames=json::array();
                for(auto& user_ : group_.getUsers())
                {
                    json userJson;
                    userJson["id"] = user_.getId();
                    userJson["name"] = user_.getName();
                    userJson["state"] = user_.getState();
                    userJson["role"] = user_.getRole();
                    userNames.push_back(userJson);
                }
                groupJson["users"] = userNames;  // 注意：需要返回GroupUser的json数据
                groupArray1.push_back(groupJson);
            }
            response["groups"] = groupArray1;
        }

        LOG_INFO<<"================================================\n";
        LOG_INFO<<response.dump()<<"\n";
        LOG_INFO<<"================================================\n";
        {
            lock_guard<mutex> lock(_keyMutex);
            //获取conn在哈希表中找到AesCrypto
            auto it = _connKeyMap.find(conn);
            if(it!=_connKeyMap.end())
            {
                sendbuf=aesEncrypt(response.dump(),it->second);
            }
            else
            {
                LOG_ERROR << "can not find aeskey for conn: " ;
            }
        }    
        //LOG_INFO<<"base之前的发送的数据长度："<<sendbuf.size()<<"\n";
        
        //LOG_INFO<<"base之后的发送的数据长度："<<sendbuf.size()<<"\n";
        //LOG_INFO<<"不使用base发送的数据长度："<<sendbuf.size()<<"\n";
        conn->send(sendbuf.c_str(),sendbuf.size());
        //生成一个
        LOG_INFO << "login success, name: " << user.getName()<<"\n";        
    }
    else
    {
        //登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        response["errmsg"] = "用户名或密码错误";//返回的错误信息
        {
            lock_guard<mutex> lock(_keyMutex);
            //获取conn在哈希表中找到AesCrypto
            auto it = _connKeyMap.find(conn);
            if(it!=_connKeyMap.end())
            {
                sendbuf=aesEncrypt(response.dump(),it->second);
            }
            else
            {
                LOG_ERROR << "can not find aeskey for conn: " ;
            }
        }            
        conn->send(sendbuf.c_str(),sendbuf.size());
        LOG_INFO << "login failed, name: " << user.getName()<<"\n";
    }    
}
//处理注册业务
void chatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    //获取js对象中的名称
    string name = js["name"];
    //获取js对象中的密码
    string password = js["password"];

    // 创建User模型
    User user;
    user.setName(name);
    user.setPassword(password);
    user.setState("offline");

    //插入到数据库中
    if (_userModel.insert(user))
    {
        // 注册成功,创建一个新的js对象，序列化消息并发送
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["id"] = user.getId();
        response["errno"] = 0;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        string sendbuf;
        {
            lock_guard<mutex> lock(_keyMutex);
            //获取conn在哈希表中找到AesCrypto
            auto it = _connKeyMap.find(conn);
            if(it!=_connKeyMap.end())
            {
                sendbuf=aesEncrypt(response.dump(),it->second);
            }
            else
            {
                LOG_ERROR << "can not find aeskey for conn: " ;
            }
        }            
        conn->send(sendbuf.c_str(),sendbuf.size());
        LOG_INFO << "register success, name: " << name<<"\n";
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        string sendbuf;
        {
            lock_guard<mutex> lock(_keyMutex);
            //获取conn在哈希表中找到AesCrypto
            auto it = _connKeyMap.find(conn);
            if(it!=_connKeyMap.end())
            {
                sendbuf=aesEncrypt(response.dump(),it->second);
            }
            else
            {
                LOG_ERROR << "can not find aeskey for conn: " ;
            }
        }            
        conn->send(sendbuf.c_str(),sendbuf.size());
        LOG_INFO << "register failed, name: " << name<<"\n";
    }
}

void chatService::clientCloseException(const TcpConnectionPtr& conn)
{
    User user;
    {
        lock_guard<mutex> lock(_connMutex);        
        for(auto& pair:_userConnMap)
        {
            if(pair.second == conn)
            {
                //从map表中删除这一条记录
                _userConnMap.erase(pair.first);           
                user.setId(pair.first);            
                break;
            }
        }
    }
    {
        lock_guard<mutex> lock(_keyMutex);
        for(auto& pair:_connKeyMap)
        {
            if(pair.first == conn)
            {
                //从map表中删除这一条记录
                _connKeyMap.erase(pair.first);
                //释放资源
                delete pair.second;                    
                break;
            }
        }        
    }            
    // 用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.unsubscribe(user.getId()); 
    //更新用户状态
    if(user.getId()!=-1)
    {
        user.setState("offline");
        _userModel.updateState(user);
    }
}

void chatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    //获取你要通话的人的id
    int toId=js["to"].get<int>();
    {
        lock_guard<mutex> lock(_connMutex);        
        auto it=_userConnMap.find(toId);
        if(it!=_userConnMap.end())
        {   
            // 用户在线-直接转发消息 服务器主动推送消息给toid表示的用户
            string sendbuf;
            {
                lock_guard<mutex> lock(_keyMutex);
                //获取conn在哈希表中找到AesCrypto
                auto it1 = _connKeyMap.find(it->second);
                if(it1!=_connKeyMap.end())
                {
                    sendbuf=aesEncrypt(js.dump(),it1->second);
                }
                else
                {
                    LOG_ERROR << "can not find aeskey for conn: " ;
                }
            }                    
            it->second->send(sendbuf.c_str(),sendbuf.size());
            return;
        }        
    }

    // 查询toid是否在线 
    User user = _userModel.query(toId);
    if (user.getState() == "online")
    {
        _redis.publish(toId, js.dump());
        return;
    }

    //用户不在线-存储离线消息
    _offlineMsgModel.insert(toId, js.dump());
}

void chatService::reset()
{
    //把online状态的用户设置为offline
    _userModel.resetState();
}

void chatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    // 获取js对象的id
    int userid=js["id"].get<int>();
    // 获取js对象的好友id
    int friendId=js["friendid"].get<int>();

    // 存储好友信息
    _friendModel.insert(userid, friendId);
}

//创建一个群聊
void chatService::createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    // 获取js对象的id
    int userid=js["id"].get<int>();
    // 获取要创建群的名称
    string groupName=js["groupname"].get<string>();
    // 获取要创建群的描述
    string groupDesc=js["groupdesc"].get<string>();

    ///因为是新创建的群所有还没有id（自动生成），也没有群成员

    //创建一个群对象
    Group group(-1,groupName,groupDesc);

    //AllGroup和GroupUser这两个表都要插入记录
    //先加入AllGroup表中
    if(_groupModel.createGroup(group))
    {
        //然后再加入GroupUser表中
        _groupModel.addGroup(userid, group.getId(),"creator");        
        LOG_INFO << "create group success, groupid: " << group.getId()<<"\n";
    }
    else
    {
        LOG_INFO<<"create group failed\n";
    }
}
//加入群聊
void chatService::addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    // 获取js对象的id
    int userid=js["id"].get<int>();
    // 获取要加入群的id
    int groupid=js["groupid"].get<int>();

    // 加入群
    _groupModel.addGroup(userid, groupid,"normal");
    LOG_INFO << "add group success, groupid: " << groupid<<"\n";     
}
//进行群聊天
void chatService::groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    // 获取js对象的id
    int userid=js["id"].get<int>();
    // 获取要聊天的群id
    int groupid=js["groupid"].get<int>();
    // 获取要在群里发送的消息
    string content=js["content"].get<string>();

    // 获取该群的所有成员id
    vector<int> groupUsers = _groupModel.queryGroupUser(userid,groupid);

    // 转发消息 服务器主动推送消息给groupid表示的群的所有成员
    {
        lock_guard<mutex> lock(_connMutex);     
        for(auto& userId:groupUsers)
        {           
            auto it=_userConnMap.find(userId);
            if(it!=_userConnMap.end())
            {
                //转发群消息
                string sendbuf;
                {
                    lock_guard<mutex> lock(_keyMutex);
                    //获取conn在哈希表中找到AesCrypto
                    auto it1 = _connKeyMap.find(it->second);
                    if(it1!=_connKeyMap.end())
                    {
                        sendbuf=aesEncrypt(js.dump(),it1->second);
                    }
                    else
                    {
                        LOG_ERROR << "can not find aeskey for conn: " ;
                    }
                }     
                it->second->send(sendbuf.c_str(),sendbuf.size());
            }
            else
            {
                // 查询toid是否在线 
                User user = _userModel.query(userId);
                if (user.getState() == "online")
                {
                    _redis.publish(userId, js.dump());
                }
                else
                {
                    // 存储离线群消息
                    _offlineMsgModel.insert(userId, js.dump());
                }
            }
        }
    }
    
    LOG_INFO << "group chat success, userid: " << userid<<"\n";     
}

void chatService::loginout(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    // 获取js对象的id
    int userid=js["id"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);        
        auto it=_userConnMap.find(userid);
        if(it!=_userConnMap.end())
        {
            //从map表中删除这一条记录
            _userConnMap.erase(it);           
        }
    }
    // 用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.unsubscribe(userid); 
    // 登出
    User user(userid,"","offline");
    _userModel.updateState(user);
    LOG_INFO << "loginout success, userid: " << userid<<"\n";
}

void chatService::handleRedisSubscribeMessage(int userid, string msg)
{
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnMap.find(userid);
    if (it != _userConnMap.end())
    {
        //转发群消息
        string sendbuf;
        {
            lock_guard<mutex> lock(_keyMutex);
            //获取conn在哈希表中找到AesCrypto
            auto it1 = _connKeyMap.find(it->second);
            if(it1!=_connKeyMap.end())
            {
                sendbuf=aesEncrypt(msg,it1->second);
            }
            else
            {
                LOG_ERROR << "can not find aeskey for conn: " ;
            }
        }     
        it->second->send(sendbuf.c_str(),sendbuf.size());
        return;
    }

    // 存储该用户的离线消息
    _offlineMsgModel.insert(userid, msg);
}

//处理客户端发送过来的密钥
void chatService::handleKey(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    //获取密钥加密后的数据
    string aeskey=js["aeskey"].get<string>();
    //获取密钥的哈希值
    string hashkey=js["aeshash"].get<string>();

    RsaCrypto rsa("private.pem",RsaCrypto::PrivateKey);
    //使用私钥解密
    string m_aesKey=rsa.priKeyDecrypt(aeskey);
    //校验哈希
    //将base64处理的文本格式数据解密为二进制格式的数据
    Base64 base;
    hashkey = base.decode(hashkey);
    Hash h(HashType::Sha224);
    h.addData(m_aesKey);
    string res=h.result(Hash::Type::Binary);
    json resJs;
    resJs["msgid"]=AES_KEY_ACK;    
    if(res!=hashkey)
    {
        LOG_INFO<<"AES key error\n";
        resJs["aeskeyOK"]=false;
        conn->send(resJs.dump());
        return;
    }
    resJs["aeskeyOK"]=true;
    conn->send(resJs.dump());
    //生成一个对称加密的对象
    LOG_INFO<<"AES key length: "<<m_aesKey.length()<<"\n";
    AesCrypto* m_aescry=new AesCrypto(AesCrypto::AES_CBC_256,m_aesKey);
    //插入到哈希表中
    _connKeyMap.insert({conn,m_aescry});
    LOG_INFO<<"AES key SUCCESS\n";    
}

//使用对称加密的密钥加密数据,并返回加密后的数据
string chatService::aesEncrypt(string data,AesCrypto* cipher)
{
    string str=cipher->encrypt(data);
    return str;
}
//对数据使用对称加密的密钥解密
string chatService::aesDecrypt(string data,AesCrypto* cipher)
{

    string str1=cipher->decrypt(data);
    return str1;
}

AesCrypto* chatService::getAesCrypto(const TcpConnectionPtr& conn)
{
    //根据conn在哈希表中找到AesCrypto            
    {
        lock_guard<mutex> lock(_keyMutex);      
        //获取conn在哈希表中找到AesCrypto
        auto it = _connKeyMap.find(conn);
        if(it!=_connKeyMap.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }            
}



