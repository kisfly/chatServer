#ifndef USERMODEL_H
#define USERMODEL_H
#include"user.hpp"
//User数据库表的数据操作类
class UserModel
{
public:
    //User表增加一个用户
    bool insert(User& user);
    //User表通过id查询一个用户
    User query(int id);
    //User表通过id修改一个用户的状态
    bool updateState(User& user);
    //重置用户的状态信息
    void resetState();
};

#endif
