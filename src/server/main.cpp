#include<iostream>
#include<signal.h>
#include"chatserver.hpp"
#include"chatservice.hpp"
#include"Logger.h"
using namespace std;

//处理服务器ctrl+c结束后，重置user的状态
chatServer *chatserver;
void Stop(int sig)    // 信号2和15的处理函数，功能是停止服务程序。
{
    signal(SIGINT, SIG_DFL);  // 重置为默认行为
    // 重置SIGINT信号的处理函数
    cout << "Caught SIGINT, exiting..." << endl;
    chatService::getInstance()->reset();  // 重置user的状态
    LOG_INFO("sig=%d\n",sig);
    // 调用BankServer::Stop()停止服务。
    chatserver->Stop();
    LOG_INFO("chatserver已停止。\n");
    delete chatserver;
    LOG_INFO("delete chatserver。\n");
}

int main(int argc, char *argv[]) {

    //signal(SIGINT, resetHanlder);  //处理SIGINT信号，防止程序结束时数据库出现错误

    if(argc<3)
    {
        cerr<<"Usage: "<<argv[0]<<" ip port"<<endl;
        exit(-1);
    }
    //解析命令行参数传递的ip和port
    char *ip=argv[1];
    uint16_t port=atoi(argv[2]);

    //main函数中初始化日志，并且使用
	if (!Logger::instance()->init("/home/fly/projects/chatServer/chatServer/log.conf"))
	{
		fprintf(stderr, "init log module failed.\n");
		return -2;
	}

    signal(SIGTERM,Stop);    // 信号15，系统kill或killall命令默认发送的信号。
    signal(SIGINT,Stop);        // 信号2，按Ctrl+C发送的信号。

    chatserver=new chatServer(ip,port,3,2);
    chatserver->Start();

    return 0;
}