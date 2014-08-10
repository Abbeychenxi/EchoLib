#ifndef ECHO_QUERY_SERVER
#define ECHO_QUERY_SERVER 

#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include "TextQuery.h"

class QueryServer : NonCopyable
{
    public:
        QueryServer(const InetAddress &addr, const std::string &filename);
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);

        void runQuery(const std::string &word, const TcpConnectionPtr &conn);

        TcpServer server_;
        TextQuery query_;
        ThreadPool pool_;
};

#endif  /*ECHO_QUERY_SERVER*/
