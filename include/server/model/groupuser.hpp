#ifndef GROUP_USER_H
#define GROUP_USER_H


#include"user.hpp"
class GroupUser:public User
{
public:
    void setRole(string role) { this->role = role; }
    string getRole() const { return role; }

private:
    string role; // 组内的用户角色：0-普通成员，1-管理员
};

#endif