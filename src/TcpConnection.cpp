#include "TcpConnection.h"
using namespace std;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)


TcpConnection::TcpConnection(int sockfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr)
    :sockfd_(sockfd),
     buffer_(sockfd),
     localAddr_(localAddr),
     peerAddr_(peerAddr)
{
}

TcpConnection::~TcpConnection()
{
    shutdown();
}

ssize_t TcpConnection::readn(char *usrbuf, size_t n)
{
    int nread = buffer_.readn(usrbuf, n); 
    if(nread == -1)
        ERR_EXIT("readn");
    return nread;
}

ssize_t TcpConnection::readLine(char *usrbuf, size_t n)
{
    int nread = buffer_.readline(usrbuf, n);
    if(nread == -1)
        ERR_EXIT("readLine");
    return nread;
}


ssize_t TcpConnection::writen(const char *usrbuf, size_t n)
{
    int nwrite = buffer_.writen(usrbuf, n);
    if(nwrite == -1)
        ERR_EXIT("write");
    return nwrite;
}

void TcpConnection::send(const std::string &s)
{
    writen(s.c_str(), s.size());
}

std::string TcpConnection::receive()
{
    char buf[1024];
    readLine(buf, 1024);
    return std::string(buf);
}

void TcpConnection::shutdown()
{
    sockfd_.shutdownWrite();
}


void TcpConnection::handleConnection()
{ 
    if(onConnectionCallback_) 
        onConnectionCallback_(shared_from_this()); 
}

void TcpConnection::handleMessage()
{ 
    if(onMessageCallback_)
        onMessageCallback_(shared_from_this()); 
}

void TcpConnection::handleClose()
{
    if(onCloseCallback_)
        onCloseCallback_(shared_from_this()); 
}





