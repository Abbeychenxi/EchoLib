#ifndef ECHO_TCP_SERVER_H
#define ECHO_TCP_SERVER_H 

#include <echo/TcpServer.h>
#include <echo/NonCopyable.h>

class EchoServer : NonCopyable
{
    public:
        EchoServer(const InetAddress &addr);
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);

        TcpServer server_;
};



#endif  /*ECHO_TCP_SERVER_H*/
