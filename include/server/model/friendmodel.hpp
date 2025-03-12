#ifndef FRIENDSERVER_MAIN_H
#define FRIENDSERVER_MAIN_H

#include<vector>
#include"user.hpp"
using namespace std;
class FriendModel
{
private:
    /* data */
public:
    //User表增加一个好友
    void insert(int userid, int friendid);

    //返回用户好友列表
    vector<User> query(int userid);
};

#endif