#include"chatservice.hpp"
#include"public.hpp"
#include<muduo/base/Logging.h>
#include<vector>
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
            conn->send(response.dump());
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
            vector<string> friendNames;
            for(auto& friend_ : friends)
            {
                json friendJson;
                friendJson["id"] = friend_.getId();
                friendJson["name"] = friend_.getName();
                friendJson["state"] = friend_.getState();
                friendNames.push_back(friendJson.dump());
            }
            response["friends"] = friendNames;
        }

        //查询该用户加入的群组信息并返回
        vector<Group> groups = _groupModel.queryGroups(id);
        if(!groups.empty())
        {
            vector<string> groupNames;
            for(auto& group_ : groups)
            {
                json groupJson;
                groupJson["id"] = group_.getId();
                groupJson["groupname"] = group_.getName();
                groupJson["groupdesc"] = group_.getDesc();
                //将vector<GroupUser>数据转为json数据
                vector<string> userNames;
                for(auto& user_ : group_.getUsers())
                {
                    json userJson;
                    userJson["id"] = user_.getId();
                    userJson["name"] = user_.getName();
                    userJson["state"] = user_.getState();
                    userJson["role"] = user_.getRole();
                    userNames.push_back(userJson.dump());
                }
                groupJson["users"] = userNames;  // 注意：需要返回GroupUser的json数据
                groupNames.push_back(groupJson.dump());
            }
            response["groups"] = groupNames;
        }

        string str=response.dump();
        conn->send(str);
        LOG_INFO << "login success, name: " << user.getName()<<"\n";        
    }
    else
    {
        //登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        response["errmsg"] = "用户名或密码错误";//返回的错误信息
        conn->send(response.dump());
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
        conn->send(response.dump());
        LOG_INFO << "register success, name: " << name<<"\n";
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;//客户端根据这个值判断：如果为0，表示成功，不为0表示失败
        conn->send(response.dump());
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
            it->second->send(js.dump());
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
                it->second->send(js.dump());
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
        it->second->send(msg);
        return;
    }

    // 存储该用户的离线消息
    _offlineMsgModel.insert(userid, msg);
}

