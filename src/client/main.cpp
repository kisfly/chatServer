#include"json.hpp"
#include<iostream>
#include <iomanip>
#include <sstream>
#include<string>
#include<vector>
#include<chrono>
#include<ctime>
#include<thread>
#include<semaphore.h>
#include<atomic>
#include<unordered_map>
using namespace std;
using json=nlohmann::json;

#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>

#include<group.hpp>
#include<user.hpp>
#include"public.hpp"

//记录当前系统登录的用户信息
User g_currentUser;
//记录当前登录用户的好友列表
vector<User> g_currentUserFriendList;
//记录当前登录用户的群组列表
vector<Group> g_currentUserGroupList;
//读写信号量，用来通知读写进程
sem_t g_rwLock;
//标识-用来标识是否登录成功-使用原子量保证互斥访问
atomic_bool isLoginSuccess{false};
//标识主界面是否运行
bool isMainRunning{false};

//显示当前登录用户的基本信息
void showCurrentUserData();

//接收线程
void readTaskHandler(int clientfd);
//获取系统时间（聊天信息需要添加时间信息）
string getCurrentTime();
//主聊天页面程序
void mainMenu(int clientfd);
//处理登录功能
void doLogin(json &js);
//处理注册功能
void doReg(json &js);

//main主线程用作发送线程，子线程用作接收线程
int main(int argc, char *argv[])
{
    if(argc<3)
    {
        cerr<<"Usage: "<<argv[0]<<" ip port"<<endl;
        exit(-1);
    }
    //解析命令行参数传递的ip和port
    char *ip=argv[1];
    uint16_t port=atoi(argv[2]);

    //创建client端的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd == -1)
    {
        cerr<<"Create socket error!"<<endl;
        exit(-1);
    }

    //填写client需要连接的server信息ip+port
    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    //连接服务器
    if(connect(clientfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        cerr<<"Connect server error!"<<endl;
        close(clientfd);
        exit(-1);
    }

     //连接服务器成功，启动接受线程负责接收数据
    thread readTask(readTaskHandler, clientfd);
    readTask.detach();
    //初始化信号量 0-表示线程间通信 1-表示进行间通信。第三个参数表示信号资源数量-初始为0  
    sem_init(&g_rwLock, 0, 0);

    //main线程用于接收用户输入，负责发送数据
    for(;;)
    {
        //显示首页面菜单 登录 注册 退出
        cout<<"========================Main Menu=========================="<<endl;
        cout<<"1. login"<<endl;
        cout<<"2. register"<<endl;
        cout<<"3. quit"<<endl;
        cout<<"=========================================================="<<endl;
        cout<<"choice: ";
        int choice;
        cin>>choice;
        cin.get();//读掉缓冲区残留的回车-以免影响下一次的读入
        switch (choice)
        {
        case 1://login业务
        {
            cout<<"user-id: ";
            int id;
            cin>>id;
            cin.get();
            cout<<"user-password: ";
            char pwd[50]={0};
            cin.getline(pwd, 50);

            //将用户信息序列化成json
            json js;
            js["msgid"] = LOGIN_MSG;
            js["id"] = id;
            js["password"] = pwd;
            isLoginSuccess = false;

            //将json转换成字符串并发送
            string buf = js.dump();
            int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
            if(len==-1)
            {
                cerr<<"send login message error: "<<buf<<endl;
            }

            //阻塞在信号量，等待读线程进行处理
            sem_wait(&g_rwLock);
            if(isLoginSuccess)
            {
                //切换到登录界面
                isMainRunning=true;
                mainMenu(clientfd);
            }    
            else
            {
                cout<<"isLoginSuccess=false-----login failed"<<endl;
            }                      
        }      
        break;
        case 2://register业务
        {
            cout<<"user-name: ";
            char name[50]={0};
            cin.getline(name, 50);

            cout<<"user-password: ";
            char pwd[50]={0};
            cin.getline(pwd, 50);

            //将用户信息序列化成json
            json js;
            js["msgid"] = REG_MSG;
            js["name"] = name;
            js["password"] = pwd;

            //将json转换成字符串并发送
            string buf = js.dump();
            int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
            if(len==-1)
            {
                cerr<<"send register message error: "<<buf<<endl;
            }     
            //阻塞在信号量，等待读线程进行处理
            sem_wait(&g_rwLock);      
        }
        break;
        case 3://quit
        {
            close(clientfd);
            //释放信号量
            sem_destroy(&g_rwLock);
            exit(0);
        }
        break;
        default:
            cerr<<"invalid input!"<<endl;
            break;
        }
    }

    return 0;


}


//显示当前登录用户的基本信息
void showCurrentUserData()
{
    cout<<"=====================login user==========================="<<endl;
    cout<<"current login user => id: "<<g_currentUser.getId()<<"  name: "<<g_currentUser.getName()<<endl;
    cout<<"=====================friend list=========================="<<endl;
    for(auto& user : g_currentUserFriendList)
    {
        cout<<user.getId()<<" "<<user.getName()<<" "<<user.getState()<<endl;        
    }
    cout<<"=====================group list==========================="<<endl;
    for(auto& group : g_currentUserGroupList)
    {
        cout<<group.getId()<<" "<<group.getName()<<" "<<group.getDesc()<<endl; 
        cout<<"group member:"<<endl;
        for(auto& user : group.getUsers())
        {
            cout<<user.getId()<<" "<<user.getName()<<" "<<user.getState()
            <<" "<<user.getRole()<<endl;    
        }
    }
    cout<<"<========================================================>"<<endl;
}
//接收线程
void readTaskHandler(int clientfd)
{
    for(;;)
    {
        char buffer[1024*10]={0};
        int len=recv(clientfd, buffer, sizeof(buffer)-1, 0);
        if(len==-1) 
        {
            //打印错误原因
            cerr<<"len=-1---readTaskHandler-recv message error: "<<strerror(errno)<<endl;
            close(clientfd);
            exit(0);
        }
        else if(len==0)
        {
            //打印错误原因
            cerr<<"len=0---readTaskHandler-recv message error: "<<strerror(errno)<<endl;
            close(clientfd);
            exit(0);
        }
        json js = json::parse(buffer);
        int msgid = js["msgid"].get<int>();
        switch (msgid)
        {
        case ONE_CHAT_MSG://单人聊天
        {
            string name = js["name"].get<string>();
            string content = js["content"].get<string>();
            cout<<"个人消息: "<<js["time"]<<" ["<<js["id"].get<int>()<<"] "
            <<name<<" said: "<<content<<endl;
        }
        break;
        case GROUP_CHAT_MSG://群聊天
        {
            cout<<"群消息["<<js["groupid"].get<int>()<<"]: "<<js["time"]<<" ["<<js["id"].get<int>()<<"] "
            <<" said: "<<js["content"].get<string>()<<endl;
        }
        break;
        case LOGIN_MSG_ACK://登录响应消息
        {
            doLogin(js);
            sem_post(&g_rwLock);
        }
        break;
        case REG_MSG_ACK://注册响应消息
        {
            doReg(js);
            sem_post(&g_rwLock);
        }
        break;
        }
    }
}
//获取系统时间（聊天信息需要添加时间信息）
string getCurrentTime()
{
    /*
     // 获取当前时间点
     auto now = std::chrono::system_clock::now();
     // 将时间点转换为 time_t 类型
     std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
 
     // 创建一个 struct tm 对象
     struct tm localTime;
     // 使用 localtime_r 函数将 time_t 类型转换为本地时间
     localtime_r(&currentTime, &localTime);
 
     // 使用 stringstream 来格式化时间
     std::ostringstream oss;
     // 设置时间格式，例如：年-月-日 时:分:秒
     oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
 
     return oss.str();
    */

     auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
     struct tm *ptm = localtime(&tt);
     char date[60] = {0};
     sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
             (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
             (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
     return std::string(date);
}

void help(int fd=0,string str="");
void chat(int,string);
void addfriend(int,string);
void creategroup(int,string);
void addgroup(int,string);
void groupchat(int,string);
void loginout(int,string);
//系统支持的客户端命令列表
unordered_map<string,string> commandMap={
    {"help","显示所有支持的命令,格式help"},
    {"chat","一对一聊天,格式chat:friendid:message"},
    {"addfriend","添加好友,格式addfriend:friendid"},
    {"creategroup","创建群组,格式creategroup:groupname:groupdesc"},
    {"addgroup","加入群组,格式addgroup:groupid"},
    {"groupchat","群聊,格式groupchat:groupid:message"},
    {"loginout","注销,格式loginout"}
};
//注册系统支持的客户端命令处理
unordered_map<string,function<void(int,string)>> commandHandlerMap={
    {"help",help},
    {"chat",chat},
    {"addfriend",addfriend},
    {"creategroup",creategroup},
    {"addgroup",addgroup},
    {"groupchat",groupchat},
    {"loginout",loginout}
};
//主聊天页面程序
void mainMenu(int clientfd)
{
    help();
    char buffer[1024]={0};
    while(isMainRunning)
    {
        cin.getline(buffer, 1024);
        string commandbuf=buffer;
        string command;//存储命令
        int idx=commandbuf.find(':');
        if(idx==-1)//没有找到:
        {
            command=commandbuf;
        }
        else{
            command=commandbuf.substr(0, idx);
        }

        auto it=commandHandlerMap.find(command);
        if(it==commandHandlerMap.end())
        {
            cerr<<"invalid command: "<<command<<endl;
            continue;
        }
        //调用相应命令的事件处理回调，mainMenu对修改封闭，添加新功能不需要修改函数
        it->second(clientfd, commandbuf.substr(idx+1,commandbuf.size()-idx));
    }
}

void help(int fd,string str)
{
    cout<<"show command list"<<endl;
    for(auto& pair : commandMap)
    {
        cout<<pair.first<<" : "<<pair.second<<endl;
    }
    cout<<endl;
}
void chat(int clientfd,string str)
{
    int idx=str.find(":");
    if(idx==-1)
    {
        cerr<<"invalid chat command: "<<str<<endl;
        return;
    }
    int friendid=atoi(str.substr(0, idx).c_str());
    string message=str.substr(idx+1,str.size()-idx);
    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["to"] = friendid;
    js["content"] = message;
    js["time"]=getCurrentTime();
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send chat message error: "<<buf<<endl;
    }
}
void addfriend(int clientfd,string str)
{
    int friendid=atoi(str.c_str());
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = g_currentUser.getId();
    js["friendid"] = friendid;
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send add friend message error: "<<buf<<endl;
    }
}
void creategroup(int clientfd,string str)
{
    int idx=str.find(":");
    if(idx==-1)
    {
        cerr<<"invalid chat command: "<<str<<endl;
        return;
    }
    string groupName=str.substr(0,idx);
    string groupDesc=str.substr(idx+1,str.size()-idx);
    json js;
    js["msgid"] = CREATE_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupname"] =groupName;
    js["groupdesc"] = groupDesc;
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send create group message error: "<<buf<<endl;
    }
}
void addgroup(int clientfd,string str)
{
    //要确保不加入重复的群
    int groupId=atoi(str.c_str());
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupid"] = groupId;
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send add group message error: "<<buf<<endl;
    }
}
void groupchat(int clientfd,string str)
{
    int idx=str.find(":");
    if(idx==-1)
    {
        cerr<<"invalid chat command: "<<str<<endl;
        return;
    }
    int groupId=atoi(str.substr(0, idx).c_str());
    string message=str.substr(idx+1,str.size()-idx);
    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = g_currentUser.getId();
    js["groupid"] = groupId;
    js["content"] = message;
    js["time"]=getCurrentTime();
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send group chat message error: "<<buf<<endl;
    }    
}
void loginout(int clientfd,string str)
{
    //通知服务器退出登录
    json js;
    js["msgid"] = LOGINOUT_MSG;
    js["id"] = g_currentUser.getId();
    string buf = js.dump();
    int len=send(clientfd, buf.c_str(), strlen(buf.c_str())+1, 0);
    if(len==-1)
    {
        cerr<<"send loginout message error: "<<buf<<endl;
    }
    else
    {
        isMainRunning=false;
    }
}

//处理登录功能
void doLogin(json &js)
{
    if(0!=js["errno"].get<int>())//登录失败
    {
        cout<<"login failed: "<<js["errmsg"].get<string>()<<endl;
        isLoginSuccess=false;
        return;
    }
    else//登录成功
    {
        //记录当前用户的id和name
        g_currentUser.setId(js["id"].get<int>());
        g_currentUser.setName(js["name"].get<string>());
        g_currentUserFriendList.clear();
        g_currentUserGroupList.clear();
        //记录当前用户的好友列表信息
        if(js.contains("friends"))
        {
            vector<string> friends = js["friends"].get<vector<string>>();
            for(auto& friendId : friends)
            {
                json friendJson = json::parse(friendId);
                User friendUser;
                friendUser.setId(friendJson["id"].get<int>());
                friendUser.setName(friendJson["name"].get<string>());
                friendUser.setState(friendJson["state"].get<string>());
                g_currentUserFriendList.push_back(friendUser);
            }
        }

        //记录当前用户的群组列表信息
        if(js.contains("groups"))
        {
            vector<string> groups = js["groups"].get<vector<string>>();
            for(auto& groupId : groups)
            {
                json groupJson = json::parse(groupId);
                Group group;
                group.setId(groupJson["id"].get<int>());
                group.setName(groupJson["groupname"].get<string>());
                group.setDesc(groupJson["groupdesc"].get<string>());

                vector<string> users = groupJson["users"].get<vector<string>>();
                for(auto& userId : users)
                {
                    json userJson = json::parse(userId);
                    GroupUser groupUser;
                    groupUser.setId(userJson["id"].get<int>());
                    groupUser.setName(userJson["name"].get<string>());
                    groupUser.setState(userJson["state"].get<string>());
                    groupUser.setRole(userJson["role"].get<string>());
                    group.getUsers().push_back(groupUser);
                }

                g_currentUserGroupList.push_back(group);
            }
        }

        //显示当前登录用户的基本信息
        showCurrentUserData();

        //显示当前用户的离线消息，个人聊天消息或者群组消息
        if(js.contains("offlinemsg"))
        {
            vector<string> offlinemsg = js["offlinemsg"].get<vector<string>>();
            cout<<"========================离线消息========================"<<endl;
            for(auto& msg : offlinemsg)
            {
                json msgJson = json::parse(msg);
                //time+[id]+name+"said:"+xxx
                if(msgJson.contains("name"))//单聊消息
                {
                    cout<<"个人消息: "<<msgJson["time"]<<" ["<<msgJson["id"].get<int>()<<"] "
                    <<msgJson["name"].get<string>()<<" said: "
                    <<msgJson["content"].get<string>()<<endl;
                }
                else//显示群聊消息
                {
                    cout<<"群消息["<<msgJson["groupid"].get<int>()<<"]: "<<msgJson["time"]<<" ["<<msgJson["id"].get<int>()<<"] "
                    <<" said: "<<msgJson["content"].get<string>()<<endl;
                }                             
            }
            cout<<"<======================================================>"<<endl;   
        }
    }
    isLoginSuccess=true;
}
//处理注册功能
void doReg(json &js)
{
    if(0!=js["errno"].get<int>())//注册失败
    {
        cerr<<"name is already exist, register failed!"<<endl;
    }
    else//注册成功
    {
        cout<<"name register success,userid is: "<<js["id"]<<endl;
    }    
}

