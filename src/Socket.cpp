#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

Socket::Socket(int sockfd)
    :sockfd_(sockfd)
{
}

Socket::~Socket()
{
    ::close(sockfd_);
}

void Socket::bindAddress(const InetAddress &addr)
{
    if(::bind(sockfd_, (SA*)addr.getSockAddrInet(), sizeof(struct sockaddr_in)) == -1)
        ERR_EXIT("bind");
}

void Socket::listen()
{
    if(::listen(sockfd_, SOMAXCONN) == -1)
        ERR_EXIT("listen");
}

int Socket::accept()
{
    int peerfd;
    if((peerfd = ::accept(sockfd_, NULL, NULL)) == -1)
        ERR_EXIT("accept");
    return peerfd;
}

void Socket::shutdownWrite()
{
    ::shutdown(sockfd_, SHUT_WR);
}

void Socket::setReusePort()
{
    int on = 1;
    if(::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &on, static_cast<socklen_t>(sizeof on)) == -1)
        ERR_EXIT("setsockopt");
}




