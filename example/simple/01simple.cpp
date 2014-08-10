#include <echo/TcpServer.h>
#include <iostream>
using namespace std;

void onConnection(const TcpConnectionPtr &conn)
{
    cout << "Client: " << conn->getPeerAddr().toIp() 
        << " Port: " << conn->getPeerAddr().toPort() << endl;
    conn->send("hello\r\n");
    //conn->shutdown();
}

void onMessage(const TcpConnectionPtr &conn)
{
    string s(conn->receive());
    conn->send("ACK\r\n");
}

int main(int argc, const char *argv[])
{
    TcpServer server(InetAddress(8989));
    server.setConnection(&onConnection);
    server.setMessage(&onMessage);
    server.start();
    return 0;
}
