#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H 

#include <echo/NonCopyable.h>
#include <echo/TcpServer.h>
#include <set>

class ChatServer : NonCopyable
{
    public:
        ChatServer(const InetAddress &addr);
        void start();

    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);

        TcpServer server_;
        std::set<TcpConnectionPtr> clients_;

        const static size_t kMaxClients = 3;
};


#endif  /*CHAT_SERVER_H*/
