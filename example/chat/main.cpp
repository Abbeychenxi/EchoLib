#include "ChatServer.h"
using namespace std;

int main(int argc, const char *argv[])
{
    ChatServer server(InetAddress(8989));
    server.start();
    return 0;
}
