#ifndef OFFLINE_MSG_HANDLER_H
#define OFFLINE_MSG_HANDLER_H
#include<iostream>
#include<vector>
using namespace std;
class offlineMsgModel
{
private:
    /* data */
public:
    //存储用户的离线消息
    void insert(int userid, string content);

    //删除用户的离线消息
    void remove(int userid);

    //查询用户的离线消息
    vector<string> query(int userid);
};



#endif