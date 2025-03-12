#include"friendmodel.hpp"
#include"db.hpp"
//User表增加一个好友
void FriendModel::insert(int userid, int friendid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO Friend values(%d, %d)", 
        userid,friendid);
    //2.执行sql语句
    MySQL db;
    //连接成功
    if(db.connect())
    {
        //执行更新语句
        db.update(sql);
    }
}

//返回用户好友列表
vector<User> FriendModel::query(int userid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    //多表联合查询
    sprintf(sql, "select a.id,a.name,a.state from User a inner join Friend b on b.friendid=a.id where b.userid=%d",userid);
   //2.执行sql语句
   MySQL db;
   //连接成功
   vector<User> contentList;
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
               User user;
               user.setId(atoi(row[0]));
               user.setName(row[1]);
               user.setState(row[2]);
               contentList.push_back(user);  //添加到列表中
           }
           //释放资源
           mysql_free_result(result);
       }
   }
   return contentList; //返回空vector
}
