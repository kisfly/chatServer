#include"groupmodel.hpp"
#include"db.hpp"
//创建一个群组
bool GroupModel::createGroup(Group& group)
{
    //1.组装sql语句
    char sql[1024] = {0};
    //第一
    sprintf(sql, "INSERT INTO AllGroup(groupname,groupdesc) values('%s', '%s')", 
        group.getName().c_str(), group.getDesc().c_str());
    //2.执行sql语句
    MySQL db;
    if(db.connect())
    {
        //执行更新语句
        if(db.update(sql))
        {
            //获取插入成功的群组数据生成的主键id
            int id = mysql_insert_id(db.getConn());
            group.setId(id);
            return true;
        }
    }
    return false; //创建失败
}
//加入群组
void GroupModel::addGroup(int userid, int groupid,string role)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO GroupUser values(%d, %d, '%s')", 
        groupid, userid, role.c_str());
    //2.执行sql语句
    MySQL db;
    if(db.connect())
    {
        //执行更新语句
        db.update(sql);
    }
}
//查询用户所在群组信息
vector<Group> GroupModel::queryGroups(int userid)
{
    /*
    1.先根据userid在groupuser表中查询出该用户所属的群组信息groupid
    2. 再根据查询出来的groupid在AllGroup表中查询出该群组的名称和描述
    */
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"select a.id,a.groupname,a.groupdesc from AllGroup a inner join \
         GroupUser b on a.id = b.groupid where b.userid=%d ",userid);
    //2.执行sql语句
    MySQL db;
    vector<Group> groups;
    if(db.connect())
    {
        //执行查询语句
        MYSQL_RES* result = db.query(sql);
        if(result)
        {
            //查询到数据
            MYSQL_ROW row;
            //查出userid所有的群组信息
            while((row = mysql_fetch_row(result)))
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groups.push_back(group);
            }
            //释放资源
            mysql_free_result(result);
        }        
    }

    //查询群组的用户信息
    for(auto& group : groups)
    {
        //1.组装sql语句
        sprintf(sql,"select a.id,a.name,a.state,b.grouprole from User a inner join \
            GroupUser b on a.id = b.userid where b.groupid=%d",group.getId());
        //2. 执行sql语句
        MYSQL_RES *res = db.query(sql);
        if(res)
        {
            //查询到数据
            MYSQL_ROW row;
            //查出userid在groupid下的所有用户信息
            while((row = mysql_fetch_row(res)))
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            //释放资源
            mysql_free_result(res);
        }        
    }
    return groups; //返回空vector
}
//根据指定的groupid查询群组用户id列表，除userid自己，主要用于群聊业务给群组其他成员群发消息
vector<int> GroupModel::queryGroupUser(int userid,int groupid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "SELECT userid FROM GroupUser WHERE groupid=%d AND userid != %d", groupid, userid);
    //2. 执行sql语句
    MySQL db;
    vector<int> userids;
    if(db.connect())
    {
        //执行查询语句
        MYSQL_RES* result = db.query(sql);
        if(result)
        {
            //查询到数据
            MYSQL_ROW row;
            //查出userid在groupid下的所有用户id
            while((row = mysql_fetch_row(result)))
            {
                userids.push_back(atoi(row[0]));
            }
            //释放资源
            mysql_free_result(result);
        }        
    }
    return userids; //返回空vector
}