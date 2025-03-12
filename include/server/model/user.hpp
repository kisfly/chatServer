#ifndef USER_H
#define USER_H

#include<string>
using namespace std;

//匹配User数据库表的OPM（映射关系对象）类
class User
{
private:
    int id;
    string name;
    string password;
    string state;//表示用户的状态
public:
    User(int id=-1, string name="", string state="offline") 
    : id(id), name(name), state(state) {}

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setState(string state) { this->state = state; }
    void setPassword(string password) { this->password = password; }

    //设置4个get方法获取对应的属性
    int getId() const { return id; }
    string getName() const { return name; }
    string getState() const { return state; }
    string getPassword() const { return password; }
};


#endif