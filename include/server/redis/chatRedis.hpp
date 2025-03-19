#ifndef CHATREDIS_H
#define CHATREDIS_H

#include<string>
#include"json.hpp"
#include"UserModel.hpp"
#include<sw/redis++/redis++.h>
//#include<sw/redis++/connection_pool.h>//连接池专用

using namespace sw::redis;
using namespace std;
using json=nlohmann::json;

class myRedis
{
public:
    myRedis()=default;
    ~myRedis() {
        if(m_redis) delete m_redis; // 改为释放 Redis 对象
    }
    //初始化连接redis++
    bool connectRedis();
    //清空数据
    void clearData();

    //向redis中插入rsa的密钥
    void insertRsaKey(const string& rsakeyType, const string& value);
    //获取RSA密钥
    string getRsaKey(const string& rsakeyType);

    //读取所有用户数据到redis中
    void loadAllUserToRedis();
    //根据用户ID获取用户所有数据
    User getUserById(int id);
    //更新用户数据
    void updateUser(User& user);
    //插入一条用户数据
    bool insertUser(User& user);
    //重置所有用户的状态
    void resetUserState();    

private:
    // 序列化/反序列化工具
    static string serialize(User& user);
    static User deserialize(string& data);

private:
    //Redis 对象指针
    Redis* m_redis = nullptr; 
    //数据操作类对象
    UserModel _userModel;
};




#endif