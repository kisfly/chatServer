#ifndef GROUP_H
#define GROUP_H

#include<string>
#include<vector>
#include"groupuser.hpp"
using namespace std;

class Group
{
public:
    Group(int id=-1, string name="", string desc="")
    {
        this->id = id;
        this->name = name;
        this->desc = desc;
    }

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setDesc(string desc) { this->desc = desc; }

    // 获取4个get方法获取对应的属性
    int getId() const { return id; }
    string getName() const { return name; }
    string getDesc() const { return desc; }

    vector<GroupUser> &getUsers() { return users; }

private:
    int id;     // 组id
    string name;// 组名
    string desc;// 组描述
    vector<GroupUser> users;// 组内的用户
};


#endif