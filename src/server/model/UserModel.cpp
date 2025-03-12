#include"UserModel.hpp"
#include"db.hpp"
#include<iostream>
using namespace std;

bool UserModel::insert(User& user)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO User(name, password , state) VALUES('%s', '%s','%s')", 
        user.getName().c_str(), user.getPassword().c_str(),user.getState().c_str());
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        if(db.update(sql))
        {
            //更新成功
            //获取插入成功的用户数据生成的主键id
            int id = mysql_insert_id(db.getConn());
            user.setId(id);
            return true;
        }
    }
    return false;
}

User UserModel::query(int id)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "SELECT * FROM User WHERE id=%d", id);
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行查询语句
        MYSQL_RES* result = db.query(sql);
        if(result)
        {
            //查询到数据
            MYSQL_ROW row = mysql_fetch_row(result);
            if(row)
            {
                //查询成功
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                //释放资源
                mysql_free_result(result);
                return user;
            }
        }
    }
    return User(); //返回空User
}

bool UserModel::updateState(User& user)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "UPDATE User SET state='%s' WHERE id=%d", 
        user.getState().c_str(), user.getId());
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        return db.update(sql);
    }
    return false;
}

void UserModel::resetState(){
    //1.组装sql语句
    char sql[1024] = "UPDATE User SET state='offline'";
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        db.update(sql);
    }
}