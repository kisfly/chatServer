#ifndef DB_H
#define DB_H

#include<mysql/mysql.h>
#include<string>
#include <ctime>
using namespace std;
 // 数据库操作类
class MySQL
{
public:
    //初始化数据库连接
    MySQL();
    //释放数据库连接资源
    ~MySQL();
    // 连接数据库
    // 连接数据库
	bool connect(string ip, 
		unsigned short port, 
		string user, 
		string password,
		string dbname);
    // 更新操作-通过slq语句更新
    bool update(string sql);
    // 查询操作
    MYSQL_RES* query(string sql);
    //获取mysql连接
    MYSQL* getConn() { return _conn; }

    // 刷新一下连接的起始的空闲时间点
	void refreshAliveTime() { _alivetime = clock(); }
	// 返回存活的时间
	clock_t getAliveeTime()const { return clock() - _alivetime; }
 private:
    MYSQL *_conn;
    clock_t _alivetime; // 记录进入空闲状态后的起始存活时间
 };


#endif
