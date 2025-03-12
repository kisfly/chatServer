#include"offlinemessagemodel.hpp"
#include"db.hpp"
//存储用户的离线消息
void offlineMsgModel::insert(int userid, string content)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO OfflineMessage values(%d, '%s')", 
        userid,content.c_str());
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        db.update(sql);
    }
}

//删除用户的离线消息
void offlineMsgModel::remove(int userid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid=%d", userid);
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        db.update(sql);
    }
}

//查询用户的离线消息
vector<string> offlineMsgModel::query(int userid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "SELECT message FROM OfflineMessage WHERE userid=%d", userid);
    //2.执行sql语句
    MySQL db;
    //连接成功
    vector<string> contentList;
    if(db.connect())
    {
        //执行查询语句
        MYSQL_RES* result = db.query(sql);
        if(result)
        {            
            //查询到数据
            MYSQL_ROW row;
            //把userid用户的所有离线消息放入contentList中并返回
            while((row = mysql_fetch_row(result))!= nullptr)
            {
                contentList.push_back(row[0]);
            }
            //释放资源
            mysql_free_result(result);
        }
    }
    return contentList; //返回空vector
}