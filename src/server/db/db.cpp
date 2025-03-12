#include"db.hpp"
#include<muduo/base/Logging.h>
using namespace muduo;
// 数据库配置信息
/*
static string server = "127.0.0.1";
static string user = "root";
static string password = "123456";
static string dbname = "chatServerDatabase";
*/
//初始化数据库连接
MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}
//释放数据库连接资源
MySQL::~MySQL()
{
    if (_conn != nullptr)
        mysql_close(_conn);
}
// 连接数据库
bool MySQL::connect(string ip, unsigned short port, 
	string username, string password, string dbname)
{
	// 连接数据库
	MYSQL *p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
    if (p != nullptr)
    {
    //设置中文支持
    mysql_query(_conn, "set names gbk");
    LOG_INFO<<"connect to database success!";
    }
    else{
    LOG_INFO<<"connect to database failed! Error:"<<mysql_error(_conn) <<  "\n";
    }
    return p;
}
// 更新操作-通过slq语句更新
bool MySQL::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
            << sql << "更新失败!";
        return false;
    }
    return true;
}
// 查询操作
MYSQL_RES* MySQL::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
            << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(_conn);
}