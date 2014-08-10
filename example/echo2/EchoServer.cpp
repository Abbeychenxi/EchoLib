#include "EchoServer.h"
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;
using namespace placeholders;

EchoServer::EchoServer(const InetAddress &addr)
    :server_(addr),
     pool_(1000, 4)
{
    server_.setConnection(bind(&EchoServer::onConnection, this, _1));
    server_.setMessage(bind(&EchoServer::onMessage, this, _1));
    server_.setClose(bind(&EchoServer::onClose, this, _1));
}

void EchoServer::start()
{
    //
    pool_.start();
    server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << endl;
    conn->send("Hello, welcome to Echo Server!!!\r\n");
}


void EchoServer::onMessage(const TcpConnectionPtr &conn)
{
    string s(conn->receive());
    //不要把recv任务放到线程池
    //conn->send(s);
    pool_.addTask(bind(&EchoServer::compute, this, s, conn));
}

//把计算任务和tcp回发做成一个函数，交给线程池
void EchoServer::compute(const std::string &word, const TcpConnectionPtr &conn)
{
    string s = word;
    for(size_t ix = 0; ix != s.size(); ++ix)    
    {
        if(isupper(s[ix]))
            s[ix] = tolower(s[ix]);
        else if(islower(s[ix]))\
            s[ix] = toupper(s[ix]);
    }
    conn->send(s);
}

void EchoServer::onClose(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << "Close" << endl;
    conn->shutdown();
}



