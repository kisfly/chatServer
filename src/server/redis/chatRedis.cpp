#include"chatRedis.hpp"
#include<muduo/base/Logging.h>
using namespace muduo;
//初始化连接redis++
bool myRedis::connectRedis()
{
    try {
        ConnectionOptions opts;
        opts.host = "127.0.0.1";
        opts.port = 6379;
        //opts.password = "your_password"; // 可选
        
        ConnectionPoolOptions pool_opts;
        pool_opts.size = 6;          // 连接池大小
        pool_opts.wait_timeout = 3s; // 获取连接超时
        pool_opts.connection_lifetime = 10min; // 连接生命周期

        // 使用 Redis 类直接管理连接池 
        m_redis = new Redis(opts, pool_opts);
        LOG_INFO<<"redis++ server connected successfully\n";
        return true;
    } catch (const Error &e) {
        LOG_ERROR<<"redis++ server connection failed\n";
        // 记录错误日志
        return false;
    }
}
//清空数据
void myRedis::clearData()
{
    //清空redis中所有数据，当redis服务器重启时清空
    m_redis->flushdb();
}

//向redis中插入rsa的密钥
void myRedis::insertRsaKey(const string& rsakeyType, const string& value)
{
    //rsakeyType-表示是公钥还是私钥
    //value-表示公钥或者私钥的文本格式数据
    m_redis->hset("RSA",rsakeyType, value);
}

//获取RSA密钥
string myRedis::getRsaKey(const string& rsakeyType)
{
    //rsakeyType-表示是公钥还是私钥
    //返回公钥或者私钥的文本格式数据
    auto value1=m_redis->hget("RSA",rsakeyType);
    //判断是否存在
    if(value1.has_value())
    {
        //LOG_INFO<<"rsa key "<<rsakeyType<<" not found\n";
        return value1.value();
    }
    LOG_ERROR<<"rsa key "<<rsakeyType<<" not found\n";
    return string();
}

//读取所有用户数据到redis中
void myRedis::loadAllUserToRedis()
{
    // 1. 查询所有id
    auto users = _userModel.getAllUser();

    // 插入到redis
    for (auto &user : users) {
         // 2. 更新缓存（双写策略）
        auto key = "user:" + to_string(user.getId());
        m_redis->setex(key, 3600, serialize(user));
    }
}

//根据用户ID获取用户所有数据
User myRedis::getUserById(int id)
{
    auto key = "user:" + to_string(id);
    try {
        // 1. 先查缓存
        if (auto val = m_redis->get(key)) {
            return deserialize(*val);
        }

        // 2. 缓存未命中则查数据库（伪代码）
        User user =_userModel.query(id);

        // 3. 写入缓存并设置过期时间
        m_redis->setex(key, 3600, serialize(user)); // 缓存1小时
        return user;
    } catch (const Error &e) {
        // 处理Redis异常
        LOG_INFO << "Redis error: " << e.what() << "\n";
        return _userModel.query(id); // 降级到直接查DB
    }
}
//更新用户数据
void myRedis::updateUser(User& user)
{
    // 1. 先更新数据库
    _userModel.updateState(user);
    //db.updateUserInDB(user);

    // 2. 更新缓存（双写策略）
    auto key = "user:" + to_string(user.getId());
    m_redis->setex(key, 3600, serialize(user));
}

//插入一条用户数据
bool myRedis::insertUser(User& user)
{
    //数据更新
    if(_userModel.insert(user))
    {
        //更新缓存
        auto key = "user:" + to_string(user.getId());
        m_redis->setex(key, 3600, serialize(user));
        return true;
    }
    return false;    
}

//重置所有用户的状态
void myRedis::resetUserState()
{
    // 1. 先更新数据库
    _userModel.resetState();
}

 // 序列化/反序列化工具
string myRedis::serialize(User& user) {
    json j;
    j["id"] = user.getId();
    j["name"] = user.getName();
    j["password"] = user.getPassword(); 
    j["state"]=user.getState();
    return j.dump();
}

User myRedis::deserialize(string& data) {
    auto j = json::parse(data);
    User user;
    user.setId(j["id"].get<int>());
    user.setName(j["name"].get<string>());
    user.setPassword(j["password"].get<string>());
    user.setState(j["state"].get<string>());
    return user;
}

















